# CPU Accelerated Math Foundation

Data types built on Single Instruction Multiple Data, SIMD, DirectXMath library of intrinsics for speed and simple implementation providing the class functionality you would expect in modern C\+\+.
Two dependencies:

DirectXMath, visit: https://github.com/Microsoft/DirectXMath

json visit: https://github.com/nlohmann/json

This code is the foundation of a fully functional DirectX 12 game engine and physics simulator.

## Classes
    Accelerated four basic data types for floating point operations on multiple data within the King namespace. Three other non-accelerated data type for ints are provided for completeness and historically is where this library had its roots (developer convirnence for multiple data pairs in user interfaces)
    
    #include "MathSIMD\MathSIMD.h"
    // SIMD
    class float2; 
    class foat3;
    class float4;
    class Quaternion;
    // not accelerated
    class uint2;
    class int2;
    class int3;
