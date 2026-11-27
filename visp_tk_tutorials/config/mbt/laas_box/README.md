# LAAS box tracking configuration

The object to track is called laas_box. It is a rectangular parallelepiped of 0.35 x 0.35 m, and 0.312 m high.

Files used by the model-based tracker (`vpMbGenericTracker`) tutorials to track a box:

| File            | Content                                                                                                                                             |
|-----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------|
| `laas_box.cao`  | 3D model of the box (8 points, 6 faces, 0.35 x 0.35 x 0.312 m), in meters                                                                           |
| `laas_box.init` | 3D coordinates of the 4 points to click to initialize the tracking                                                                                  |
| `laas_box.jpg`  | Image showing where to click the initialization points                                                                                              |
| `laas_box.xml`  | Tracker settings, XML format (used by `launch/mbt_laas_box_bag_xml_launch.py`, commented inline)                                                    |
| `laas_box.json` | Tracker settings, JSON format (used by `launch/mbt_laas_box_bag_json_launch.py`, described below) |

`laas_box.json` cannot contain comments: ViSP 3.7.0 parses it with a strict JSON parser, and a comment would
make the loading of the configuration fail. Comments will be introduced in `laas_box.json` with ViSP > 3.7.0

## `laas_box.json` keys

Distances are in meters, angles in degrees, image quantities in pixels.
The equivalent `laas_box.xml` tag is given when there is one.

### Top level

| Key                   | Value     | Meaning                                                                                  |
|-----------------------|-----------|------------------------------------------------------------------------------------------|
| `version`             | `"1.0"`   | Version of the settings format, required by ViSP.                                        |
| `referenceCameraName` | `"Color"` | Camera in which the pose is expressed. Must be one of the keys of `trackers`.           |
| `trackers`            | object    | One entry per camera. The key is the camera name, here a single `Color` camera.         |
| `vvs`                 | object    | Settings of the virtual visual servoing that computes the pose (see below).             |

### `trackers.Color`

| Key              | Meaning                                                                                                    | XML equivalent              |
|------------------|------------------------------------------------------------------------------------------------------------|-----------------------------|
| `model`          | Path to the 3D model. `package://` is replaced by the share directory of the package.                     | (none: given to the node)   |
| `type`           | Visual features used by this camera: `edge` (moving edges) and `klt` (keypoints). `depthDense` and `depthNormal` also exist for depth cameras. | (none: given to the node) |
| `camTref`        | 4x4 homogeneous matrix from the reference camera to this camera. Identity here, because this camera is the reference. | -                |
| `angleAppear`    | A face becomes visible when the angle between its normal and the line of sight is lower than this value. | `face/angle_appear`         |
| `angleDisappear` | A face stops being visible when this angle is greater than this value (hysteresis with `angleAppear`).    | `face/angle_disappear`      |

#### `camera`

Intrinsic parameters, perspective model without distortion: principal point `u0`, `v0` and focal lengths `px`, `py`, in pixels.
XML equivalent: `camera`.

The `visp_mbt` node overrides these values with the intrinsics received on the `camera_info` topic.

#### `clipping`

| Key     | Meaning                                                                                                       | XML equivalent              |
|---------|---------------------------------------------------------------------------------------------------------------|-----------------------------|
| `near`  | Parts of the model closer than this distance are ignored.                                                     | `face/near_clipping`        |
| `far`   | Parts of the model farther than this distance are ignored.                                                    | `face/far_clipping`         |
| `flags` | Clipping planes to use: `all` enables near, far and field-of-view clipping.                                  | `face/fov_clipping`         |

#### `display`

| Key               | Meaning                                                              |
|-------------------|----------------------------------------------------------------------|
| `features`        | Display the features used by the tracker (edge samples, keypoints). |
| `projectionError` | Display the projection error of the model.                           |

#### `edge`: moving edges

| Key             | Meaning                                                                                                                    | XML equivalent                    |
|-----------------|----------------------------------------------------------------------------------------------------------------------------|-----------------------------------|
| `maskSize`      | Size of the convolution masks used to detect the edges.                                                                    | `ecm/mask/size`                   |
| `nMask`         | Number of masks, i.e. of quantized edge orientations (180 = one mask per degree).                                          | `ecm/mask/nb_mask`                |
| `range`         | Search range on each side of a sample, along the normal to the projected edge.                                             | `ecm/range/tracking`              |
| `thresholdType` | `normalized` (recommended) or `old` (absolute threshold).                                                                  | `ecm/contrast/edge_threshold_type` (1 = normalized) |
| `threshold`     | Minimal likelihood for a point to be an edge. With `normalized`, it is a gray level difference (0-255): the lower, the more (but less reliable) points. | `ecm/contrast/edge_threshold` |
| `mu`            | Contrast tolerances `[mu1, mu2]`: a point is rejected if its contrast differs from the previous one by more than these ratios. | `ecm/contrast/mu1`, `mu2`     |
| `sampleStep`    | Distance between two consecutive samples along a projected edge.                                                           | `ecm/sample/step`                 |

#### `klt`: keypoints

| Key             | Meaning                                                                 | XML equivalent      |
|-----------------|-------------------------------------------------------------------------|---------------------|
| `maskBorder`    | Margin from the borders of a face in which no keypoint is detected.     | `klt/mask_border`   |
| `maxFeatures`   | Maximal number of keypoints tracked.                                    | `klt/max_features`  |
| `windowSize`    | Size of the window used by the KLT tracker around each keypoint.       | `klt/window_size`   |
| `quality`       | Minimal quality of a corner, relative to the best corner of the image. | `klt/quality`       |
| `minDistance`   | Minimal distance between two detected keypoints.                        | `klt/min_distance`  |
| `harris`        | Free parameter k of the Harris corner detector.                         | `klt/harris`        |
| `blockSize`     | Size of the averaging block used to compute the corner score.           | `klt/size_block`    |
| `pyramidLevels` | Number of pyramid levels (helps for large motions).                     | `klt/pyramid_lvl`   |

#### `lod`: level of detail

| Key                              | Meaning                                                                         |
|----------------------------------|---------------------------------------------------------------------------------|
| `useLod`                         | If `true`, lines and faces that look too small in the image are not tracked.    |
| `minLineLengthThresholdGeneral`  | With `useLod`, minimal length, in pixels, of a projected line to be tracked.    |
| `minPolygonAreaThresholdGeneral` | With `useLod`, minimal area, in pixels², of a projected face to be tracked.     |

#### `visibilityTest`

| Key        | Meaning                                                                                       |
|------------|-----------------------------------------------------------------------------------------------|
| `scanline` | Use a scanline rendering to find the visible parts of the model (handles self-occlusions).  |
| `ogre`     | Use Ogre3D ray tracing to test face visibility (requires ViSP built with Ogre).              |

### `vvs`: pose computation

| Key         | Meaning                                                                      |
|-------------|------------------------------------------------------------------------------|
| `lambda`    | Gain of the virtual visual servoing.                                         |
| `maxIter`   | Maximal number of iterations per image.                                      |
| `initialMu` | Initial damping factor of the Levenberg-Marquardt optimization.              |

## Differences between `laas_box.xml` and `laas_box.json`

The two files do not use the same values for:

- the camera intrinsics (`px` = 839 in XML, 605 in JSON). This does not matter in practice,
  because the node overrides them with the `camera_info` topic;
- the far clipping distance (100 m in XML, 10 m in JSON).
