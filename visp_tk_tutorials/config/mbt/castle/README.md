# Castle tracking configuration

The object to track is called castle. It is the textured castle also tracked by the RBT
(`config/rbt/models/castle`).

Files used by the model-based tracker (`vpMbGenericTracker`) to track the castle with an RGB-D camera:

| File            | Content                                                                             |
|-----------------|-------------------------------------------------------------------------------------|
| `castle.cao`    | 3D model of the castle, in meters. It only loads the 4 parts of `castle_parts/`     |
| `castle_parts/` | The 4 parts of the model, one `.cao` file each (see below)                          |
| `castle.init`   | 3D coordinates of the 4 points to click to initialize the tracking                  |
| `castle.jpg`    | Image showing where to click the initialization points                              |
| `castle.json`   | Tracker settings, JSON format (used by `launch/mbt_castle_live_rs2_json_launch.py`) |

When the tracking is initialized by click, ViSP saves the resulting pose in `castle.0.pos`, next to `castle.init`
in the install share directory. At the next initialization, this pose is displayed first: left click to accept it,
right click to click the 4 points again.

## `castle.cao`

The RBT model, `castle_rework.obj`, is a scanned mesh of about 36 000 triangles. The MBT cannot use it: it would
track an edge on each triangle, and the faces would be too small for the KLT and depth features. `castle.cao` is a
simplified model, made of the planar parts of the castle that are seen from the front. It contains no point nor face
itself: it includes the 4 files of `castle_parts/` with `load()`, a path relative to `castle.cao`.

The object frame is not the one of the RBT model. Its Y axis is vertical, upwards. The Z axis points to the front of
the castle, on the side of the door. The X axis points from the floor to the tower. Each part is a single named face:

| File                                 | Face name                                                | Content                                                                                                                 |
|--------------------------------------|----------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------|
| `castle_parts/castle_floor.cao`      | `floor`                                                  | Floor on the left of the tower, horizontal at Y = 0.08076 (6 points)                                                    |
| `castle_parts/castle_tower.cao`      | `tower_front`, `tower_left`, `tower_right`, `tower_back` | The 4 walls of the tower, X in [-0.043, 0.0406], Y in [0.08076, 0.17876], Z in [-0.043, 0.039] (no top nor bottom face) |
| `castle_parts/castle_front_door.cao` | `front_door`                                             | Front wall of the floor, with the door, vertical at Z = 0.02945 (6 points)                                              |
| `castle_parts/castle_slope.cao`      | `slope`                                                  | Slope on the right of the tower, X in [0.04055, 0.07855], going down from the back to the front                         |

The battlements, the walls around the floor and the rocks are not modeled.

## `castle.init`

The 4 points to click, shown on `castle.jpg`, are corners of the model:

| Point | Location                                                      | Corner of the model         |
|-------|---------------------------------------------------------------|-----------------------------|
| 1     | Front corner of the floor, far from the tower                 | `castle_floor.cao` point 00 |
| 2     | Bottom front corner of the left face of the tower             | `castle_tower.cao` point 01 |
| 3     | Top front corner of the left face of the tower, above point 2 | `castle_tower.cao` point 00 |
| 4     | Top back corner of the left face of the tower                 | `castle_tower.cao` point 05 |

## `castle.json`

`castle.json` is based on `teabox.json`, described in [`../teabox/README.md`](../teabox/README.md). It combines two
cameras of a RealSense:

- `Color`: the color camera, tracked with moving edges and KLT keypoints, with the same settings as `teabox.json`;
- `Depth`: the depth camera, tracked with `depthDense` features.

The `Depth` values that differ from `teabox.json` are:

| Key            | `castle.json`    | `teabox.json`     | Effect                                                                                                                                                                                                                                                                          |
|----------------|------------------|-------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `type`         | `["depthDense"]` | `["depthNormal"]` | Each depth point sampled on the visible faces is a feature (point-to-plane distance), instead of one plane per face estimated from the point cloud. The castle has few and partly occluded planar faces: the many dense points constrain the pose better when the object moves. |
| `clipping.far` | 2.0              | 5.0               | Depth points further than 2 m are ignored.                                                                                                                                                                                                                                      |
| `lod.useLod`   | false            | true              | All the faces are used, whatever their size in the image.                                                                                                                                                                                                                       |

The dense sampling step (`dense.sampling`, 4 x 4 pixels) and `clipping.near` (0.01 m) are the same in both files.
The `normals` settings are kept, but are not used with `depthDense`.

As for the teabox, `camTref` is the identity in the file: the extrinsics between the depth and color cameras are
read from the TFs published by the RealSense driver, using the `reference_tracker` and `other_tracker` launch
arguments of `launch/mbt_castle_live_rs2_json_launch.py`.
