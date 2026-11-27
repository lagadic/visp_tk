# Render-Based Tracker configurations

JSON configuration files of the ViSP Render-Based Tracker (`vpRBTracker`), used by the `visp_rbt` node.
They are given to the node with the `config_file` launch argument, for instance:

```
ros2 launch visp_tk_tutorials rbt_orangina_live_rs2_launch.py \
  config_file:=$(ros2 pkg prefix visp_tk_tutorials)/share/visp_tk_tutorials/config/rbt/configs/depth-ccd-mask.json
```

The files contain no comments: their parameters are described below. The 3D model is not given in these files,
but by the `model_file` launch argument.

| File                               | Features                                    | Mask | Used by default by                                      |
|------------------------------------|---------------------------------------------|------|---------------------------------------------------------|
| `depth-ccd-mask.json`              | `depth`, `silhouetteColor`                  | yes  | `rbt_bag_launch.py`, `rbt_dragon_live_rs2_launch.py`, `rbt_teabox_live_rs2_launch.py`, `rbt_stomach_live_rs2_launch.py` |
| `depth-ccd.json`                   | `depth`, `silhouetteColor`                  | no   |                                                         |
| `depth-klt-ccd-mask.json`          | `depth`, `klt`, `silhouetteColor`           | yes  | `rbt_cube_live_rs2_launch.py`                           |
| `depth-klt.json`                   | `depth`, `klt`                              | no   |                                                         |
| `depth-klt-me-single.json`         | `silhouetteMe` (1 candidate), `depth`, `klt`| no   |                                                         |
| `depth-klt-me-multi.json`          | `silhouetteMe` (3 candidates), `depth`, `klt`| no  |                                                         |
| `ccd.json`                         | `silhouetteColor`                           | no   |                                                         |
| `ccd-temporal-smoothing.json`      | `silhouetteColor`, smoothed over time       | no   |                                                         |
| `orangina-depth-klt-ccd-mask.json` | `depth`, `silhouetteColor`, `klt` + drift   | yes  | `rbt_orangina_live_rs2_launch.py`                       |
| `castle-depth-klt-ccd-mask.json`   | `depth`, `klt`, `silhouetteColor`           | yes  | `rbt_castle_live_rs2_launch.py`                         |

`ccd.json` and `ccd-temporal-smoothing.json` only use the color image: they do not require a depth stream.
All the other files require a depth image aligned with the color image.

## Choosing a configuration

- **`depth` + `silhouetteColor`** (`depth-ccd*.json`): depth aligns the model on the measured surface, CCD aligns
  the silhouette on the color contrast between the object and the background. Good default for untextured objects.
- **`klt`** (`depth-klt*.json`): keypoints tracked on the object texture. Needed when the object has a symmetry
  that depth and silhouette cannot observe, e.g. the rotation of a can around its axis.
- **`silhouetteMe`** (`depth-klt-me-*.json`): moving edges along the silhouette. More precise than CCD to
  localize a contour, but sensitive to the edges of reflections and of a cluttered background.
- **Mask** (`*-mask.json`): rejects the features lying on the background or on occluding objects.
  Without a `mask` section, the `useMask` options of the features have no effect.
- **Drift detection** (`orangina-depth-klt-ccd-mask.json`): computes a confidence score, displayed by the node
  as `Drift score`.

Warnings:

- `depth-klt-ccd-mask.json`, `depth-klt.json` and `depth-klt-me-*.json` set `vvs.mu` to 0 (no damping), with up
  to 50 iterations for the last three. For an object with a symmetry, the KLT has no tracked points at the first
  frame: a degree of freedom is then not constrained and the pose may diverge.
- `depth-klt.json` has no silhouette feature: the tracker does not check that the object is still in the image
  before the optimization.

## Keys

Distances are in meters, image quantities in pixels.

### Top level

| Key                            | Meaning                                                                                        |
|--------------------------------|------------------------------------------------------------------------------------------------|
| `vvs`                          | Settings of the pose optimization (see below). Required.                                       |
| `silhouetteExtractionSettings` | How the silhouette points are extracted from the render of the model (see below). Required.   |
| `features`                     | List of the features used in the optimization (see below). Required.                           |
| `mask`                         | Optional. Per-pixel probability that a pixel belongs to the object (see below).               |
| `drift`                        | Optional. Drift detection (see below).                                                         |

### `vvs`: pose optimization

The pose is estimated at each image by a Levenberg-Marquardt like virtual visual servoing.

| Key              | Meaning                                                                                                   |
|------------------|-----------------------------------------------------------------------------------------------------------|
| `gain`           | Gain applied to the velocity computed at each iteration.                                                  |
| `maxIterations`  | Maximum number of iterations per image.                                                                   |
| `mu`             | Initial damping factor. It limits large pose updates at the first iterations; 0 means no damping (Gauss-Newton). |
| `muIterFactor`   | The damping factor is multiplied by this value at each iteration.                                         |
| `scaleInvariant` | If `true`, the damping is scaled by the diagonal of the Hessian approximation.                            |

### `silhouetteExtractionSettings`

The silhouette (contour of the object) is extracted from the depth discontinuities of the render of the model
at the current pose. Its points are used by the `silhouetteColor` and `silhouetteMe` features.

| Key                            | Meaning                                                                                  |
|--------------------------------|------------------------------------------------------------------------------------------|
| `threshold.type`, `value`      | `relative`: a depth discontinuity of the render is a contour when it is larger than `value` times the depth extent of the object (distance between its nearest and farthest rendered points). With any other type (e.g. `absolute`), `value` is a distance in meters. |
| `sampling.samplingRate`        | Keep one silhouette pixel out of `samplingRate` as candidate.                            |
| `sampling.numPoints`           | Maximum number of silhouette points (0 = no limit).                                      |
| `sampling.reusePreviousPoints` | Keep the points of the previous image when possible, for temporal consistency.          |

### `mask`: object segmentation

| Key                              | Meaning                                                                                                  |
|----------------------------------|----------------------------------------------------------------------------------------------------------|
| `type`                           | `joint`: combination (minimum) of a depth-based mask (`depth`) and a color-based mask (`color`). `depth` or `histogram` alone are also possible. |
| `depth.minRadiusFactor`          | A pixel whose measured depth is within the depth range of the rendered object, enlarged by this factor times the object radius, belongs to the object. Pixels with no depth are also considered as belonging to the object. |
| `depth.falloffRadiusFactor`      | Outside this range, the probability decreases smoothly, over a distance proportional to this factor.   |
| `color.type`                     | `histogram`: object and background color histograms, learnt online.                                     |
| `color.bins`                     | Number of bins per color channel.                                                                        |
| `color.objectUpdateRate`         | Learning rate of the object histogram.                                                                   |
| `color.backgroundUpdateRate`     | Learning rate of the background histogram.                                                               |
| `color.maxDepthError`            | Only the pixels whose measured depth is within this distance of the rendered depth are used to learn the object histogram. |
| `color.computeOnlyOnBoundingBox` | If `true`, the mask is only computed in the bounding box of the object (faster).                         |

### `drift`: drift detection

| Key    | Meaning                                                                                                                            |
|--------|------------------------------------------------------------------------------------------------------------------------------------|
| `type` | `probabilistic`: compares the colors and depths of 3D points stored on the object surface with the current observations, and gives a confidence score. |

### `features`: keys common to all the features

| Key                 | Meaning                                                                                                     |
|---------------------|-------------------------------------------------------------------------------------------------------------|
| `type`              | `depth`, `silhouetteColor`, `silhouetteMe` or `klt` (see below). It is also the name displayed by the node in the `Features:` lines. |
| `weight`            | Weight of the feature in the optimization: either a number (constant weight), or an object giving a weight that varies over the iterations (see below). |
| `dofs`              | Optional. Degrees of freedom `[tx, ty, tz, rx, ry, rz]` estimated by this feature, in the object frame. Default: all. |
| `useMask`           | Use the `mask` to ignore the points that probably do not belong to the object. No effect without `mask` section. |
| `minMaskConfidence` | With `useMask`, points whose mask probability is lower than this value are ignored.                       |
| `display`           | Display the feature in the images.                                                                          |

A `weight` object defines a sigmoid going from `minWeight` to `maxWeight` as the optimization progresses
(progress = iteration / `maxIterations`):

| Key                | Meaning                                                                         |
|--------------------|---------------------------------------------------------------------------------|
| `minWeight`        | Weight at the beginning of the optimization (with `increasing: true`).         |
| `maxWeight`        | Weight at the end of the optimization (with `increasing: true`).               |
| `midpointLocation` | Progress, between 0 and 1, at which the weight is halfway between the two.     |
| `slopePower`       | Steepness of the sigmoid around the midpoint (1 = gradual).                     |
| `increasing`       | `true`: from `minWeight` to `maxWeight`; `false`: the other way around.         |

It permits for instance to let depth and KLT align the object first, and the silhouette refine the contour at
the last iterations.

### `depth`: dense depth

Point-to-plane distance between the measured depth and the rendered model.

| Key    | Meaning                                                                              |
|--------|--------------------------------------------------------------------------------------|
| `step` | Use one pixel out of `step` in each direction (2 = 4 times fewer points than 1).    |

### `silhouetteColor`: CCD

Contracting Curve Density: aligns the silhouette with the color contrast between the object and the background.

| Key                 | Meaning                                                                                         |
|---------------------|-------------------------------------------------------------------------------------------------|
| `temporalSmoothing` | Part of the color statistics of the previous image kept in the current ones (0 = no smoothing). |
| `maxNumPoints`      | Maximum number of silhouette points used (0 = all).                                             |
| `ccd.h`             | Half length of the segments, normal to the contour, where the colors are sampled on each side. |
| `ccd.delta_h`       | Sampling step along these segments.                                                             |
| `ccd.min_h`         | `h` is halved over the iterations, down to this value (`min_h` = `h`: constant length).        |

### `silhouetteMe`: moving edges

For each silhouette point, searches the strongest gray level edge along the normal to the rendered contour.

| Key                        | Meaning                                                                                  |
|----------------------------|------------------------------------------------------------------------------------------|
| `numCandidates`            | Number of edge candidates kept per silhouette point. Several candidates are more robust to a cluttered background. |
| `movingEdge.maskSize`      | Size of the convolution masks used to detect the edges.                                  |
| `movingEdge.nMask`         | Number of masks, i.e. of quantized edge orientations (180 = one per degree).             |
| `movingEdge.range`         | Search range on each side of the contour, along its normal.                              |
| `movingEdge.sampleStep`, `minSampleStep` | Sampling step along the contour, and its minimum value.                  |
| `movingEdge.strip`         | Margin at the image border where no edge is searched.                                    |
| `movingEdge.mu`            | Contrast tolerances `[mu1, mu2]` with respect to the previous image.                    |
| `movingEdge.thresholdType` | `normalized`: the threshold is a gray level difference, in [0, 255].                     |
| `movingEdge.threshold`     | Minimum edge strength to accept an edge. Higher values ignore weak edges (e.g. reflections). |

### `klt`: keypoints

Points detected on the object texture and tracked from image to image by optical flow.

| Key                          | Meaning                                                                         |
|------------------------------|---------------------------------------------------------------------------------|
| `maxReprojectionErrorPixels` | Tracked points further than this from their reprojection are removed.          |
| `newPointsMinPixelDistance`  | Minimum distance between a new point and the already tracked ones.             |
| `minimumNumPoints`           | New points are detected when fewer points are tracked.                          |
| `maxFeatures`                | Maximum number of detected points.                                              |
| `minDistance`                | Minimum distance between detected points.                                       |
| `quality`                    | Minimum corner quality, relative to the best corner of the image.               |
| `useHarris`, `harris`        | Use the Harris detector instead of Shi-Tomasi, and its free parameter k.        |
| `blockSize`                  | Size of the block used to compute the corner score.                             |
| `windowSize`                 | Size of the optical flow search window.                                         |
| `pyramidLevels`              | Number of pyramid levels of the optical flow (helps for large motions).         |

## Values that differ between the files

### `vvs` and silhouette extraction

| File                               | `maxIterations` | `mu` | `muIterFactor` | `samplingRate` | `numPoints` | `reusePreviousPoints` |
|------------------------------------|-----------------|------|----------------|----------------|-------------|-----------------------|
| `depth-ccd-mask.json`              | 10              | 0.1  | 0.25           | 1              | 512         | true                  |
| `depth-ccd.json`                   | 10              | 0.1  | 0.25           | 1              | 512         | true                  |
| `depth-klt-ccd-mask.json`          | 10              | 0    | 0.25           | 2              | 512         | true                  |
| `depth-klt.json`                   | 50              | 0    | 0.25           | 1              | 512         | true                  |
| `depth-klt-me-single.json`         | 50              | 0    | 0.25           | 1              | 4096        | true                  |
| `depth-klt-me-multi.json`          | 50              | 0    | 0.25           | 1              | 1024        | true                  |
| `ccd.json`                         | 25              | 0.1  | 0.25           | 1              | 512         | false                 |
| `ccd-temporal-smoothing.json`      | 25              | 0.1  | 0.1            | 1              | 512         | true                  |
| `orangina-depth-klt-ccd-mask.json` | 10              | 0.1  | 0.25           | 1              | 512         | true                  |
| `castle-depth-klt-ccd-mask.json`   | 10              | 0    | 0.25           | 2              | 512         | true                  |

All the files use `gain` = 1, `scaleInvariant` = false and a `relative` silhouette threshold of 0.1.
The files with a mask all use the same `mask` settings.

### Features

| File                               | `depth`                                | `silhouetteColor`                              | `silhouetteMe`                       | `klt`               |
|------------------------------------|----------------------------------------|------------------------------------------------|--------------------------------------|---------------------|
| `depth-ccd-mask.json`              | weight 0 → 1, step 1, mask ≥ 0.75      | weight 0.1, h 32 → 16, mask ≥ 0.75             |                                      |                     |
| `depth-ccd.json`                   | weight 0 → 1, step 1                   | weight 0.1, h 32 → 16                          |                                      |                     |
| `depth-klt-ccd-mask.json`          | weight 0 → 1, step 2, mask ≥ 0.75      | weight 0 → 0.01, h 16, mask ≥ 0.75             |                                      | weight 1, mask ≥ 0.75 |
| `depth-klt.json`                   | weight 1, step 1                       |                                                |                                      | weight 1            |
| `depth-klt-me-single.json`         | weight 1, step 1                       |                                                | weight 0 → 0.1 (midpoint 0.75), 1 candidate | weight 1     |
| `depth-klt-me-multi.json`          | weight 1, step 1                       |                                                | weight 1, 3 candidates               | weight 1            |
| `ccd.json`                         |                                        | weight 0.1, h 32 → 8                           |                                      |                     |
| `ccd-temporal-smoothing.json`      |                                        | weight 1, h 16, smoothing 0.1, 512 points      |                                      |                     |
| `orangina-depth-klt-ccd-mask.json` | weight 0 → 1, step 2, mask ≥ 0.5, no rz | weight 0.1, h 32 → 16, mask ≥ 0.5, no rz      |                                      | weight 1, mask ≥ 0.5 |
| `castle-depth-klt-ccd-mask.json`   | weight 0 → 1, step 4, mask ≥ 0.75      | weight 0 → 0.01, h 16, mask ≥ 0.75             |                                      | weight 1, mask ≥ 0.75, 1000 points |

"weight a → b": weight increasing over the iterations (midpoint 0.5 unless specified).
"mask ≥ x": `useMask` with `minMaskConfidence` x. "no rz": `dofs` without the rotation around the Z axis.
The `silhouetteMe` and `klt` settings not listed here are the same in all the files.

## `orangina-depth-klt-ccd-mask.json`

Tuned for the Orangina can (145 mm x 57 mm):

- The can is symmetric around its axis (Z axis of the object frame): its silhouette and depth do not change when it
  rotates around Z. The `depth` and `silhouetteColor` features thus do not estimate this rotation (`dofs` without
  `rz`). Only the `klt` feature, which tracks points of the printed label, constrains it.
- The can is dark blue and shiny, and the depth is missing on the specular reflections: `minMaskConfidence` is
  lowered to 0.5 (0.75 in `depth-ccd-mask.json`), so that fewer features are rejected.
- `mu` = 0.1 (instead of 0 in `depth-klt-ccd-mask.json`) limits the pose updates at the first frame, when the KLT has
  no tracked points yet.
- The drift detection displays a confidence score in the node window.

## `depth-klt-ccd-mask.json`

Used for the textured cube. Measured with a RealSense D455 at 640x480 and the cube at about 40 cm, limiting the number
of silhouette points (`silhouetteExtractionSettings.sampling.numPoints`) to 512 instead of 0 (no limit) reduces the
median tracking time from about 116 ms to 20 ms, with the same drift score (about 0.89): without limit, CCD used about
300 000 features along the silhouette, against about 50 000 with 512 points.

## `castle-depth-klt-ccd-mask.json`

`depth-klt-ccd-mask.json` tuned to reduce the tracking time on the castle, whose silhouette (battlements, towers) is
long and very jagged. Measured with a RealSense D455 at 640x480 and the castle at about 40 cm, the median tracking time
goes from about 200 ms to 50 ms, with the same drift score (about 0.89):

| Change                                                         | Median tracking time | Why                                                                                     |
|----------------------------------------------------------------|----------------------|-----------------------------------------------------------------------------------------|
| `depth-klt-ccd-mask.json` with `numPoints` = 0 (no limit)      | 199 ms               | CCD used about 440 000 features along the whole silhouette.                             |
| `depth-klt-ccd-mask.json` (`numPoints` = 512)                  | 58 ms                | CCD uses about 25 000 features.                                                         |
| `depth.step`: 2 → 4, `klt.maxFeatures`: 5000 → 1000            | 51 ms                | 4 times fewer depth points.                                                             |
| `mask.color.computeOnlyOnBoundingBox`: false → true            | 50 ms                | The color mask is only computed around the object.                                      |

Further reducing the number of silhouette points, the CCD sampling or the number of iterations only saves 1 to 2 ms.
Removing the mask makes the tracking slower (59 ms), since more features, lying on the background, are kept.

