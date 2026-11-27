# Cube tracking configuration

The object to track is called cube. It is a textured cube with 0.085 m edges, centered on the origin of the object
frame. It is the same cube as the one tracked by the RBT (`config/rbt/models/cube`), with the same object frame and
initialization points.

Files used by the model-based tracker (`vpMbGenericTracker`) to track the cube with an RGB-D camera:

| File        | Content                                                                                       |
|-------------|-----------------------------------------------------------------------------------------------|
| `cube.cao`  | 3D model of the cube (8 points, 6 faces, 0.085 m edges), in meters                            |
| `cube.init` | 3D coordinates of the 4 points to click to initialize the tracking                            |
| `cube.jpg`  | Image showing where to click the initialization points                                        |
| `cube.json` | Tracker settings, JSON format (used by `launch/mbt_cube_live_rs2_json_launch.py`)             |

When the tracking is initialized by click, ViSP saves the resulting pose in `cube.0.pos`, next to `cube.init`
in the install share directory. At the next initialization, this pose is displayed first: left click to accept it,
right click to click the 4 points again.

## `cube.json`

`cube.json` has the same settings as `teabox.json`, described in
[`../teabox/README.md`](../teabox/README.md). Only the `model` key differs. It combines two cameras of a RealSense:

- `Color`: the color camera, tracked with moving edges and KLT keypoints (the drawings on the faces of the cube give
  many keypoints);
- `Depth`: the depth camera, tracked by fitting the planes of the faces of the model to the point cloud
  (`depthNormal`).

As for the teabox, `camTref` is the identity in the file: the extrinsics between the depth and color cameras are
read from the TFs published by the RealSense driver, using the `reference_tracker` and `other_tracker` launch
arguments of `launch/mbt_cube_live_rs2_json_launch.py`.
