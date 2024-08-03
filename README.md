# CPU Accelerated Math Foundation

Data types built on Single Instruction Multiple Data, SIMD, DirectXMath library of intrinsics for speed and simple implementation providing the class functionality you would expect in modern C\+\+.
Two dependencies:


## Classes
    Unique identifiers aliased to your liking, produced is for HLSL code style of float3, int4, etc.
    The following accelerated classes are defined in the namespace King:
    
    #include "MathSIMD\MathSIMD.h"

    class FloatPoint2; // SIMD
    class FloatPoint3; // SIMD
    class FloatPoint4; // SIMD
    class Quaternion; // SIMD

    class UIntPoint2; // not accelerated
    class IntPoint2; // not accelerated
    class IntPoint3; // not accelerated
