![Banner](https://s-christy.com/sbs/status-banner.svg?icon=image/flare&hue=30&title=Ray%20Tracer&description=A%20physically-based%20ray%20tracing%20engine%20written%20in%20C%2B%2B)

## Overview

This is a physically-based ray tracing engine written in C++17 that renders 3D
scenes to PNG images. It implements the core algorithms of ray tracing including
recursive reflections, shadow casting, and Blinn-Phong shading with multiple
light sources. The engine supports spheres and infinite planes as primitives, and
includes a checkerboard procedural texture.

## Features

- Recursive ray tracing with configurable depth
- Blinn-Phong shading (ambient, diffuse, and specular)
- Hard shadows via shadow rays
- Reflective materials
- Point lights with distance attenuation
- Sphere and infinite plane primitives
- Procedural checkerboard texture
- Sky gradient background
- PNG output via stb_image_write

## Usage

```
make        # Build the project
make run    # Build and render
make clean  # Remove build artifacts
```

Output is written to `build/output.png`.

## Dependencies

## License

This work is licensed under the GNU General Public License version 3 (GPLv3).

[<img src="https://s-christy.com/status-banner-service/GPLv3_Logo.svg" width="150" />](https://www.gnu.org/licenses/gpl-3.0.en.html)
