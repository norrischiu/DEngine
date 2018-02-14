# D-ENGINE
D-ENGINE is a project developed by Norris Chiu (Sung Hong), Jack Hui (Shui Kit) and Lui Ka Fai.

D-ENGINE aims to be an open source deferred rendering game engine that is simple and easy to use, so users can learn native game programming with good exposure to the low level system, written by C++ and with core Direct3D API only from scratch.

A demo is included in the repository in the GameCode folder.

Notable features include:

<b>Graphics</b>
- Deferred shading and GBuffer
- Optimized light volume rendering with stenciling
- Custom effect classes as interfaces to GPU
- Bump, basic shadow mapping
- Screen space reflection approximation

<b>Animation</b>
- Skeletal animation implementation
- Animation State Machine
- Cross-fade blending

<b>Advanced shader</b>
- Geometry shader with stream-out supported particle system
- Terrain rendering with level of details tessellation

<b>Engine</b>
- Pool memory allocator with Handle
- Custom Handle array and hash map
- SIMD maths library​
- Game object component architecture
- Collision detection facilities
- Custom event queue and handler
- FBX asset exporter

# Building D-ENGINE
There are two ways to build the D-ENGINE and the provided demo.

1. Use the existing Microsoft Visual Studio Solution (.sln) directly.
2. Generate Microsoft Visual Studio Solution (.sln) from Premake script. For user with Microsoft Visual Studio 2015, bring up the command prompt and run premake5.exe with the command "premake5 vs2015" at the base directory of DEngine. For further information on using and extending Premake, visit their official repository https://github.com/premake/premake-core.

*GameCode should be set as the start-up project, D-ENGINE currently support Windows, Microsoft Visual Studio 2015 (v140 platform toolset is required) with Intel CPU only

# Roadmap
We planned to extend D-ENGINE in the following areas in the future:

1. Turn into a multi-threaded engine
2. Optimize GPU resources management
3. Optimize rendering with spatial data structure
4. Experiment with forward plus and other rendering technique
5. Add support for in game profiling

# License
The MIT License (MIT)
Copyright (c) 2015-2016 Norris Chiu, Jack Hui, Lui Ka Fai

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Assets
The assets (including 3D model and texture) used in the demo are downloaded from the following source:

1. Characters from Adobe Mixamo, www.mixamo.com
2. The Atrium Sponza Palace, Dubrovnik by Frank Meinl, Crytek
