# Physically based rendering (PBR)

Brings more physical principles into rendering.

Based on Microfacet theory and Phong shading model.
When computing the diffuse and specular components for a fragment,
use a weighting function (BRDF) to evaluate the contribution of a certain light source.
Sum up all those contributions to obtain the final color of the fragment.

# License

The MIT License (MIT)

Copyright (c) 2021, Jiang Ye

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Result

With direct light (e.g. point, directional and spotlight)

![result](./result/result.png)

![result2](./result/result2.png)

# Reference

[LearnOpenGL: PBR](https://learnopengl.com/PBR/Theory)

[LearnOpenGL: Normal Mapping](https://learnopengl.com/Advanced-Lighting/Normal-Mapping)

[LearnOpenGL sample code: PBR](https://github.com/JoeyDeVries/LearnOpenGL/tree/master/src/6.pbr)

[Texture resource](3dtextures.me)
