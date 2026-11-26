# Teabox tracking configuration

The object to track is called teabox. It is a rectangular parallelepiped of 0.165 x 0.068 m, and 0.08 m high.

Files used by the model-based tracker (`vpMbGenericTracker`) tutorials to track a teabox with an RGB-D camera:

| File          | Content                                                                                       |
|---------------|-----------------------------------------------------------------------------------------------|
| `teabox.cao`  | 3D model of the teabox (8 points, 6 faces, 0.165 x 0.068 x 0.08 m), in meters                 |
| `teabox.init` | 3D coordinates of the 4 points to click to initialize the tracking                            |
| `teabox.png`  | Image showing where to click the initialization points                                        |
| `teabox.json` | Tracker settings, JSON format (used by `launch/mbt_teabox_live_rs2_json_launch.py`, described below) |
| `teabox.xml`  | Color tracker settings, XML format (not used by the launch files of this package)             |

When the tracking is initialized by click, ViSP saves the resulting pose in `teabox.0.pos`, next to `teabox.init`
in the install share directory. At the next initialization, this pose is displayed first: left click to accept it,
right click to click the 4 points again.

`teabox.json` cannot contain comments: ViSP 3.7.0 parses it with a strict JSON parser, and a comment would
make the loading of the configuration fail. Comments will be introduced in `teabox.json` with ViSP > 3.7.0

## `teabox.json` keys

Distances are in meters, angles in degrees, image quantities in pixels.

Unlike `box.json`, which only uses the color camera, `teabox.json` combines two cameras of a RealSense:

- `Color`: the color camera, tracked with moving edges and keypoints;
- `Depth`: the depth camera, tracked by fitting the planes of the model faces to the point cloud.

Both contribute to the same pose, expressed in the `Color` camera frame.
When tracking, the node displays `Requires depth : true` and the number of features of each type on the right of the image.

### Top level

| Key                   | Value     | Meaning                                                                                                |
|-----------------------|-----------|--------------------------------------------------------------------------------------------------------|
| `version`             | `"1.0"`   | Version of the settings format, required by ViSP.                                                      |
| `referenceCameraName` | `"Color"` | Camera in which the pose is expressed. Must be one of the keys of `trackers`.                         |
| `model`               | path      | 3D model shared by all the cameras. `package://` is replaced by the share directory of the package. Unlike `box.json`, the model is given once here and not per camera; setting both is an error. |
| `trackers`            | object    | One entry per camera: `Color` and `Depth`.                                                             |
| `vvs`                 | object    | Settings of the virtual visual servoing that computes the pose (see below).                           |

### Keys common to both cameras

| Key              | Meaning                                                                                                    |
|------------------|------------------------------------------------------------------------------------------------------------|
| `type`           | Features used by this camera: `edge` and `klt` for a color camera, `depthNormal` and `depthDense` for a depth camera. A color type cannot be mixed with a depth type in the same camera. |
| `camTref`        | 4x4 homogeneous matrix from the reference camera to this camera. Identity in the file for both cameras: see [Extrinsics](#extrinsics-depth-to-color). |
| `camera`         | Intrinsic parameters (`px`, `py`, `u0`, `v0`), perspective model without distortion. The node overrides them with the `camera_info` topic of each camera. |
| `angleAppear`    | A face becomes visible when the angle between its normal and the line of sight is lower than this value. |
| `angleDisappear` | A face stops being visible when this angle is greater than this value (hysteresis with `angleAppear`).    |
| `clipping`       | `near` and `far` distances beyond which the model is ignored; `flags: ["all"]` also enables the field-of-view clipping. |
| `display`        | `features`: display the features used by the tracker; `projectionError`: display the projection error.   |
| `lod`            | Level of detail: with `useLod: true`, lines shorter than `minLineLengthThresholdGeneral` pixels and faces smaller than `minPolygonAreaThresholdGeneral` pixels² are not tracked. |
| `visibilityTest` | `scanline: true` uses a scanline rendering to find the visible parts of the model (handles self-occlusions); `ogre` uses Ogre3D ray tracing (requires ViSP built with Ogre). |

### `trackers.Color`

Same keys and meaning as `trackers.Color` in [`../box/README.md`](../box/README.md), which also gives the XML equivalent of each key.
The values that differ from `box.json` are:

| Key                | `teabox.json` | `box.json` | Effect                                                                         |
|--------------------|---------------|------------|--------------------------------------------------------------------------------|
| `clipping.far`     | 0.9           | 10.0       | The teabox is no longer tracked when it is more than 0.9 m from the camera.    |
| `edge.range`       | 8             | 10         | Smaller search range along the normal to the edges: faster, but supports smaller motions. |
| `edge.sampleStep`  | 4.0           | 3.0        | Fewer edge samples.                                                            |
| `edge.threshold`   | 20.0          | 10.0       | Only stronger edges are kept: fewer but more reliable points.                  |

`lod.useLod` is `false`: all the edges and faces are tracked, whatever their size in the image.

### `trackers.Depth`

| Key                        | Value         | Meaning                                                                                       |
|----------------------------|---------------|-----------------------------------------------------------------------------------------------|
| `type`                     | `depthNormal` | For each visible face, the plane of the face is estimated from the point cloud, and the pose is corrected so that the model face matches this plane. |
| `clipping.near`, `far`     | 0.01, 5.0     | Range of the depth camera.                                                                    |
| `lod.useLod`               | `true`        | Faces smaller than 2500 pixels² in the depth image are not used: their plane would be estimated from too few points. |

#### `normals`: settings of `depthNormal`

| Key                       | Value      | Meaning                                                                                  |
|---------------------------|------------|------------------------------------------------------------------------------------------|
| `featureEstimationMethod` | `"robust"` | How the plane of a face is estimated from its 3D points: `robust` (robust estimation of the plane normal and distance), `robustSVD` (robust SVD plane fitting), or `pcl` (PCL plane segmentation, requires ViSP built with PCL). |
| `sampling.x`, `sampling.y`| 2, 2       | Only one pixel every 2 columns and every 2 rows of the depth image is used. Increase to go faster with fewer points. |
| `pcl.method`              | 2          | PCL sample consensus method (`pcl::SAC_MSAC`). Only used when `featureEstimationMethod` is `"pcl"`. |
| `pcl.ransacMaxIter`       | 200        | Maximal number of RANSAC iterations. Only used with `"pcl"`.                              |
| `pcl.ransacThreshold`     | 0.001      | Maximal distance, in meters, from a point to the plane to be an inlier. Only used with `"pcl"`. |

#### `dense`: settings of `depthDense`

| Key                       | Value | Meaning                                                                         |
|---------------------------|-------|---------------------------------------------------------------------------------|
| `sampling.x`, `sampling.y`| 4, 4  | Pixel step in the depth image, as for `normals.sampling`.                       |

These settings are ignored here, because `type` does not contain `depthDense`: the node therefore displays
`Features: depth dense 0`. Add `"depthDense"` to `type` to also make each 3D point of the faces match the model
(more accurate, but slower).

### Extrinsics: depth to color

`trackers.Depth.camTref` should be the transformation from the color camera to the depth camera. It is left to identity
in the file, because `launch/mbt_teabox_live_rs2_json_launch.py` sets the launch arguments `reference_tracker` and `other_tracker`:

```
reference_tracker: [Color, camera_color_optical_frame]
other_tracker:     [Depth, camera_depth_optical_frame]
```

The node then reads the transformation between these two frames on the TF2 topics published by the RealSense driver
(`publish_tf: true`), and replaces `camTref`. The first element of each pair must match a key of `trackers`.
If you use `teabox.json` without these launch arguments, set `camTref` to the extrinsics of your camera.

### `vvs`: pose computation

| Key         | Meaning                                                                      |
|-------------|------------------------------------------------------------------------------|
| `lambda`    | Gain of the virtual visual servoing.                                         |
| `maxIter`   | Maximal number of iterations per image.                                      |
| `initialMu` | Initial damping factor of the Levenberg-Marquardt optimization.              |

## Differences between `teabox.xml` and `teabox.json`

`teabox.xml` only describes the color camera. Compared to `trackers.Color` in `teabox.json`, it uses different values for:

- the camera intrinsics (`px` = 839 in XML, 605 in JSON). This does not matter in practice,
  because the node overrides them with the `camera_info` topic;
- the far clipping distance (100 m in XML, 0.9 m in JSON).
