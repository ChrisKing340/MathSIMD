/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Title:          MathSIMD

Description:    Multiple data set variable types defined to simplify code for
                2D and 3D points and common math operations in Major 1 release.
                Operator overloads and single varible tracking was the driving
                reasons for building these classes within our engine.

                Data parallelism accelerated code through implementing SIMD
                (single instruction, multiple data) math libraries in Major 2.
                Only floating point updated at this time as most used and greatest
                performance gain data types.
                The popular DirectXMath library for intrinsics and fall back
                code is used to provide the intrinsics.  Note that this math
                library does not require DirectX and is render system independent.

                This code is intended to be complied for 64 bit operating systems
                although alignment allocations are made to be backwards compatible
                on 32 bit operating systems.

                Version 2.9.0 added the SystemInfo class to report system capabilites
                and compatabilies of intrinsics (although all modern systems support
                at some level, so this is a reporting only function and not conditional)

Contact:        ChrisKing340@gmail.com

References:     https://msdn.microsoft.com/en-us/library/windows/desktop/hh437833(v=vs.85).aspx

MIT License

Copyright (c) 2010 - 2026 Christopher H. King

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
#pragma once
#ifndef _ENABLE_EXTENDED_ALIGNED_STORAGE
#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#endif

#ifndef __cplusplus
#error MathSIMD requires C++
#endif

#define KING_MATH_VERSION_MAJOR 2
#define KING_MATH_VERSION_MINOR 9
#define KING_MATH_VERSION_PATCH 3

/*
    Change Log:

    Version 1.0     uint2, int2, int3 keeping two variables and overloading math 
    2010            operators to simplify coding/readability for GUI point math functions

    Version 1.1     float2, float3 introduced for transforming 2D points in GUI
    2011

    Version 1.2     float4 introduced for transforms of 3D points
    2011

    Version 1.3     Added methods and other variable interoporabilities to each class
    not recorded

    Version 1.4     Added move symatics optimizations to be used with STL containers
    2017            

    Version 1.5.0   Quaternionerions introduced for better rotations of 3D points. This works really
    not recorded    well, however, note that the errors in position was not gimbal lock which
                    had potential under euler math transforms, but rather floating point error.
                    Therefore, it is important to zero when near zero and not allow the error
                    to build up over time. Use operator bool() to check for angle present (tiny
                    angles are ignored to stop tiny transforms and further error introduction)
                    rather than rotating by the Quaternionerion every frame

    Version 1.6.0   Added stream support
    2018

    Version 2.0.0   Data parallelism accelerated code through SIMD (single instruction, 
    2019            multiple data) math library of DirectXMath (not dependent on DirectX)
                    for float2, float3, float4, and Quaternion. Type defines implemented to match
                    HLSL naming convention

    Version 2.3.0   Added Json support using 3rd party code by nlohmann
    2019

    Version 2.4.0   Added operator bool() and !bool() for float2, float3, float4
    21MAR2021       checking if valid/invalid (NaN) after calculations.

    Version 2.5.0   Added to float3 GetXZ, GetXY, GetYZ methods
    05FEB2022       and Added to float2 GetMagnitudeEst()

    Version 2.5.1   deprecated GetAngle() within Quaternion class for GetAngleEuler()
    09SEP2022       in the range [-π , +π] radians or GetAngleQuaternion()
                    in the range [0 , +π] radians. GetAngle() behavior was GetAngleQuaternion()
                    confusing users that want [-π , +π] and did not realize Quaternions are 
                    different. This depreciation removes the ambiguity.

    Version 2.5.2   Ensured noexcept was implemented throughout on the major 4 (copy, move, 
    30OCT2022       copy assign, move assign) so that the STL containers would use those 
                    methods optimally. Updated new operator to throw and delete to noexcept 
                    for uint2, int2, int3 

    Version 2.6.0   Separated the code from GeometryKing library as a stand alone static project.
    23JAN2023       Used by PhysicsKing which was also separated from GeometryKing as a static project.
                    Intended to allow use in a variety of new projects without the main reason it was 
                    created for (Geometry2D UI and later Geometry3D models. CPU side acceleration was
                    implemented later as the code base needed optimization and obviously this is good
                    practice for all types of math instructions that have multiple data)

    Version 2.7.0   Added to float2 DotProduct(const float2 vecIn), CrossProduct(const float2 vecIn)
    12JUL2023       and ProjectOnToVector(const float2 vecIn) similar to float3 implementation.
                    these were not used in original use of the library in 2D for GUI but now necessary
                    as it is being used for 2D physic problem solving.

    Version 2.7.1   Adjusted operator bool() and !bool() for float2, float3, float4 conversion to also check for infinite
    22SEP2023       in addition to nan.

    Version 2.8.0   Added test methods IsZero() and IsBelow(epsilon) to float2, float3, and float4.that retrun true 
    01OCT2023       if exactly zero or within epsilon.
                    Added methods Zero() and ZeroIfNearZero(epsilon) to float2, float3, and float4.which zero the vector 
                    componets (if < epsilon).
                    Added method float4::CrossProduct(...) 
                    
    Version 2.9.0   Added class SystemInfo to report to cout the cpu, graphics card, and installed system memory
    27NOV2023       Modified the new() and delete() methods for uint2, int2, and int3 for
                    multiple compiler use. Also added additional constexpr constructors and Set(...) method definitions
                    to all for compile time use in macros and templates (use case for UI in code templates)
 
    Version 2.9.1   Fixed a bug in Clamp(...) methods that was only working correctly if min < max
    10OCT2025       Fixed a bug in uint2 operator= (const DirectX::XMVECTOR & vecIn) that was not converting float to uint correctly
                    Fixed a bug in int2 operator= (const DirectX::XMVECTOR & vecIn) that was not converting float to int correctly
                    Fixed a bug in int3 operator= (const DirectX::XMVECTOR & vecIn) that was not converting float to int correctly
                    Added GetLength() to uint2, int2, int3

    Version 2.9.2   Added missing const noexcept to uint2, int2, int3 comparators to work with constants
    11JAN2026

    Version 2.9.3   Modified SystemInfo to return strings rather than print to cout for better use in UI and other reporting functions
    07FEB2026       Added SystemInfo::GetSystemInfo() to return a struct of all system info data for use in UI and other reporting functions
                    Added SystemInfo::GetCPUInfo(), GetGPUInfo(), and GetMemoryInfo() to return strings of each type of system info data for use in UI and other reporting functions
                    Added SystemInfo::GetCPUInfoStruct(), GetGPUInfoStruct(), and GetMemoryInfoStruct() to return structs of each type of system info data for use in UI and other reporting functions

    Version 3.0.0   Planned for future major release:
    31JAN2026       Renamed classes to match previous type defines for HLSL naming convention (float2, float3, float4, uint2, int2, int3)
    *** TO DO ***
    planned         - uint2 class modified for SIMD acceleration. External behavior remains the same.
    planned         - int2 class modified for SIMD acceleration. External behavior remains the same.
    planned         - int3 class modified for SIMD acceleration. External behavior remains the same.
 */

#include "..\json\single_include\nlohmann\json.hpp"
using json = nlohmann::json;

#include <windows.h>
#include <memory>
#include <new>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <DirectXMath.h>
#include <emmintrin.h>
#include <ostream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <sal.h>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <string>
#include <cassert>
#include <unordered_set>

// Windows-only additions for SystemInfo
#include <intrin.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>
#include <initguid.h>
#include <devpkey.h>
#include <devguid.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <winver.h>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "version.lib")

namespace King {
    // Unique data types built on Single Instruction Multiple Data, SIMD, DirectXMath library of intrinsics 

    class uint2; // not accelerated
    class int2; // not accelerated
    class int3; // not accelerated
    class float2; // SIMD
    class float3; // SIMD
    class float4; // SIMD
    class Quaternion; // SIMD

    // macros
#define ISNAN(x)  (bool)((*(const uint32_t*)&(x) & 0x7F800000) == 0x7F800000 && (*(const uint32_t*)&(x) & 0x7FFFFF) != 0)

    /******************************************************************************
    *   uint2
    *       Original class not adapted to DirectX intrinsics for acceleration
    *       to keep two unsigned ints and treat as one data type
    ******************************************************************************/
    class alignas(16) uint2
    {
        /* variables */
    public:
        unsigned int        u[2];
    protected:

    private:

        /* methods */
    public:
        // Creation/Life cycle
#if defined(_MSC_VER)
        void* operator new (std::size_t size) noexcept(false) { auto ptr = ::operator new(size, std::align_val_t{ 16 }); if (!ptr) throw std::bad_alloc(); return ptr; }
        void  operator delete (void* p) noexcept { ::operator delete(p, std::align_val_t{ 16 }); }
#elif defined(__GNUC__) || defined(__clang__)
        void* operator new(std::size_t size) noexcept(false) { void* ptr; if (posix_memalign(&ptr, 16, size) != 0) { throw std::bad_alloc(); } return ptr; }
        void operator delete(void* p) noexcept { free(p); }
#endif
        inline uint2() { SetZero(); }
        inline uint2(const unsigned int& xy) { Set(xy); }
        inline uint2(const unsigned long& xy) { Set(xy); }
        inline uint2(const long& xy) { Set(xy); }
        inline uint2(const unsigned long& x, const unsigned long& y) { Set(x, y); }
        inline uint2(const unsigned int& x, const unsigned int& y) { Set(x, y); }
        inline uint2(const long& x, const long& y) { Set(x, y); }
        inline uint2(const float& x, const float& y) { Set(x, y); }
        inline uint2(const int& x, const int& y) { Set(x, y); }
        inline uint2(const uint2 & in) = default; // copy
        inline uint2(const DirectX::XMUINT2 & in) { Set(in); }
        inline uint2(const int2 & in);
        inline uint2(const float2 & in);
        inline uint2(uint2 && in) = default; // move
        inline uint2(const DirectX::FXMVECTOR & vecIn) { DirectX::XMStoreInt2(u, vecIn); }
        virtual ~uint2() = default;
        // Operators 
        inline uint2& operator= (const uint2 & in) = default; // copy assignment
        inline uint2& operator= (const DirectX::XMUINT2 & in) { Set(in); return *this; } // copy assignment
        inline uint2& operator= (const DirectX::XMVECTOR & vecIn) { DirectX::XMUINT2 d; DirectX::XMVECTOR v = DirectX::XMConvertVectorFloatToUInt(vecIn, 0); DirectX::XMStoreUInt2(&d, v); Set(d); return *this; } 
        inline uint2& operator= (uint2 && in) = default; // move assignment
        // Conversions
        inline explicit operator bool() const { return (((u[0] != 0) || (u[1] != 0))); } // non-zero
        inline bool operator !() const { return ((u[0] == 0) && (u[1] == 0)); } // empty
        inline unsigned int& operator[](int idx) { return GetPtr()[idx]; }
        inline operator DirectX::XMVECTOR() const { DirectX::XMVECTORU32 r; r.u[0] = u[0]; r.u[1] = u[1]; r.u[2] = r.u[3] = 0; return r; }
        inline operator DirectX::XMUINT2() const { return Get_XMUINT2(); }
        inline operator DirectX::XMFLOAT2() const { return Get_XMFLOAT2(); }
        inline const POINT                      Get_POINT() const { POINT pt = { static_cast<long>(u[0]), static_cast<long>(u[1]) }; return pt; }
        inline const DirectX::XMINT2            Get_XMINT2() const { DirectX::XMINT2 rtn; rtn = { static_cast<int>(u[0]), static_cast<int>(u[1]) }; return rtn; }
        inline const DirectX::XMUINT2           Get_XMUINT2() const { DirectX::XMUINT2 rtn; rtn = { static_cast<unsigned int>(u[0]), static_cast<unsigned int>(u[1]) }; return rtn; }
        inline const DirectX::XMFLOAT2          Get_XMFLOAT2() const { DirectX::XMFLOAT2 rtn; rtn = { static_cast<float>(u[0]), static_cast<float>(u[1]) }; return rtn; }
        // Comparators
        inline bool operator==  (const uint2& rhs) const noexcept { return u[0] == rhs.u[0] && u[1] == rhs.u[1]; }
        inline bool operator!=  (const uint2& rhs) const noexcept { return u[0] != rhs.u[0] || u[1] != rhs.u[1]; }
        inline bool operator< (const uint2& rhs) const noexcept { return u[0] < rhs.u[0] && u[1] < rhs.u[1]; }
        inline bool operator> (const uint2& rhs) const noexcept { return u[0] > rhs.u[0] && u[1] > rhs.u[1]; }
        inline bool operator<= (const uint2& rhs) const noexcept { return u[0] <= rhs.u[0] && u[1] <= rhs.u[1]; }
        inline bool operator>= (const uint2& rhs) const noexcept { return u[0] >= rhs.u[0] && u[1] >= rhs.u[1]; }
        // Math Operators
        inline uint2 operator+ (const uint2 p) const { return uint2(u[0] + p.u[0], u[1] + p.u[1]); }
        inline uint2 operator- (const uint2 p) const { return uint2(u[0] - p.u[0], u[1] - p.u[1]); }
        inline uint2 operator* (const uint2 p) const { return uint2(u[0] * p.u[0], u[1] * p.u[1]); }
        inline uint2 operator/ (const uint2 p) const { return uint2(u[0] / p.u[0], u[1] / p.u[1]); }

        inline uint2& operator+= (const unsigned int s) { u[0] += s; u[1] += s; return *this; }
        inline uint2& operator-= (const unsigned int s) { u[0] -= s; u[1] -= s; return *this; }
        inline uint2& operator*= (const unsigned int s) { u[0] *= s; u[1] *= s; return *this; }
        inline uint2& operator/= (const unsigned int s) { u[0] /= s; u[1] /= s; return *this; }

        inline uint2& operator+= (const uint2 & p) { u[0] = u[0] + p.u[0]; u[1] = u[1] + p.u[1]; return *this; }
        inline uint2& operator-= (const uint2 & p) { u[0] = u[0] - p.u[0]; u[1] = u[1] - p.u[1]; return *this; }
        inline uint2& operator*= (const uint2 & p) { u[0] = u[0] * p.u[0]; u[1] = u[1] * p.u[1]; return *this; }
        inline uint2& operator/= (const uint2 & p) { u[0] = u[0] / p.u[0]; u[1] = u[1] / p.u[1]; return *this; }

        inline uint2 operator+  (unsigned int s) const { return uint2(u[0] + static_cast<UINT>(s), u[1] + static_cast<UINT>(s)); }
        inline uint2 operator-  (unsigned int s) const { return uint2(u[0] - static_cast<UINT>(s), u[1] - static_cast<UINT>(s)); }
        inline uint2 operator*  (unsigned int s) const { return uint2(u[0] * static_cast<UINT>(s), u[1] * static_cast<UINT>(s)); }
        inline uint2 operator/  (unsigned int s) const { return uint2(u[0] / static_cast<UINT>(s), u[1] / static_cast<UINT>(s)); }

        inline uint2 operator+  (unsigned long s) const { return uint2(u[0] + static_cast<UINT>(s), u[1] + static_cast<UINT>(s)); }
        inline uint2 operator-  (unsigned long s) const { return uint2(u[0] - static_cast<UINT>(s), u[1] - static_cast<UINT>(s)); }
        inline uint2 operator*  (unsigned long s) const { return uint2(u[0] * static_cast<UINT>(s), u[1] * static_cast<UINT>(s)); }
        inline uint2 operator/  (unsigned long s) const { return uint2(u[0] / static_cast<UINT>(s), u[1] / static_cast<UINT>(s)); }

        inline uint2 operator+  (float s) const { return uint2(static_cast<float>(u[0]) + (s), static_cast<float>(u[1]) + (s)); }
        inline uint2 operator-  (float s) const { return uint2(static_cast<float>(u[0]) - (s), static_cast<float>(u[1]) - (s)); }
        inline uint2 operator*  (float s) const { return uint2(static_cast<float>(u[0])* (s), static_cast<float>(u[1])* (s)); }
        inline uint2 operator/  (float s) const { return uint2(static_cast<float>(u[0]) / (s), static_cast<float>(u[1]) / (s)); }

        // Assignments
        inline void constexpr                   SetZero(void) { u[0] = u[1] = 0; }
        inline void                             SetX(const unsigned long x) { u[0] = static_cast<unsigned int>(x); }
        inline void                             SetY(const unsigned long y) { u[1] = static_cast<unsigned int>(y); }
        inline void                             Set(const unsigned int xy) { u[0] = u[1] = xy; }
        inline void                             Set(const unsigned long xy) { u[0] = u[1] = static_cast<unsigned int>(xy); }
        inline void                             Set(const long xy) { u[0] = u[1] = static_cast<unsigned int>(xy); }
        inline void                             Set(const float xy) { u[0] = u[1] = static_cast<unsigned int>(xy); }
        inline void                             Set(const unsigned long x, const unsigned long y) { u[0] = x; u[1] = y; }
        inline void                             Set(const unsigned int x, const unsigned int y) { u[0] = static_cast<unsigned int>(x); u[1] = static_cast<unsigned int>(y); }
        inline void                             Set(const int x, const int y) { u[0] = static_cast<unsigned int>(x); u[1] = static_cast<unsigned int>(y); }
        inline void                             Set(const long x, const long y) { u[0] = static_cast<unsigned int>(x); u[1] = static_cast<unsigned int>(y); }
        inline void                             Set(const float x, const float y) { u[0] = static_cast<unsigned int>(x); u[1] = static_cast<unsigned int>(y); }
        inline void                             Set(const uint2 & in) { *this = in; }
        inline void                             Set(const DirectX::XMUINT2 & point) { u[0] = point.x; u[1] = point.y; }
        inline void                             Set(const POINT & point) { u[0] = static_cast<unsigned int>(point.x); u[1] = static_cast<unsigned int>(point.y); }
        // Tests
        bool                                    IsZero() const { return (u[0] == 0u && u[1] == 0u); }
        // Accessors
        unsigned int*                           GetPtr() { return reinterpret_cast<unsigned int*>(this); }
        inline const auto&                      GetX() const { return u[0]; }
        inline const auto&                      GetY() const { return u[1]; }

        inline auto                             GetMagnitude() const { return (UINT)sqrt((u[0] * u[0]) + (u[1] * u[1])); } // standard math naming
        inline auto                             GetLength() const { return GetMagnitude(); } // common naming
        // Functionality
        inline void                             Min(const uint2& in) { u[0] = u[0] < in.u[0] ? u[0] : in.u[0]; u[1] = u[1] < in.u[1] ? u[1] : in.u[1]; }
        inline void                             Max(const uint2& in) { u[0] = u[0] > in.u[0] ? u[0] : in.u[0]; u[1] = u[1] > in.u[1] ? u[1] : in.u[1]; }
        //Statics
        static const float                      Magnitude(const uint2& pIn) { return static_cast<float>(std::sqrt(pIn.u[0] * pIn.u[0] + pIn.u[1] * pIn.u[1])); }
    };
    /******************************************************************************
    *   int2
    *       Original class not adapted to DirectX intrinsics for acceleration
    *       to keep two signed ints and treat as one data type
    ******************************************************************************/
    class alignas(16) int2
    {
        /* variables */
    public:
        signed int          i[2] = { 0, 0 };
    protected:

    private:

        /* methods */
    public:
        // Creation/Life cycle
#if defined(_MSC_VER)
        void* operator new (std::size_t size) noexcept(false) { auto ptr = ::operator new(size, std::align_val_t{ 16 }); if (!ptr) throw std::bad_alloc(); return ptr; }
        void  operator delete (void* p) noexcept { ::operator delete(p, std::align_val_t{ 16 }); }
#elif defined(__GNUC__) || defined(__clang__)
        void* operator new(std::size_t size) noexcept(false) { void* ptr; if (posix_memalign(&ptr, 16, size) != 0) { throw std::bad_alloc(); } return ptr; }
        void operator delete(void* p) noexcept { free(p); }
#endif
        inline constexpr int2() {}
        inline constexpr int2(const long xy) { Set(xy); }
        inline constexpr int2(const long x, const long y) { Set(x, y); }
        inline constexpr int2(const int x, const int y) { Set(x, y); }
        inline constexpr int2(const unsigned int x, const unsigned int y) { Set(x, y); }
        inline constexpr int2(const float x, const float y) { Set(x, y); }
        inline constexpr int2(const unsigned long x, const unsigned long y) { Set(x, y); }
        inline int2(const int2 & in) = default; // copy
        inline int2(const uint2 & in) { Set(in); }
        inline int2(const float2 & in);
        inline int2(const POINT & in) { Set(in); }
        inline int2(const DirectX::XMINT2 & in) { Set(in); }
        inline int2(int2 && in) = default; // move
        inline explicit int2(const DirectX::XMVECTOR vecIn) { DirectX::XMINT2 t; DirectX::XMStoreSInt2(&t, vecIn); i[0] = t.x; i[1] = t.y; } // integer vector input NOT float
        virtual ~int2() = default;
        // Operators 
        inline int2& operator= (const int2 & in) = default; // copy assignment
        inline int2& operator= (const DirectX::XMINT2 & in) { Set(in); return *this; } // copy assignment
        inline int2& operator= (const DirectX::XMVECTOR & vecIn) { unsigned int d[2]; auto v = DirectX::XMConvertVectorFloatToInt(vecIn, 0); DirectX::XMStoreInt2(d, v); i[0] = (long)d[0]; i[1] = (long)d[1]; return *this; }
        inline int2& operator= (int2 && in) = default; // move assignment
        // Conversions
        inline explicit operator bool() const { return ((i[0] != 0) || (i[1] != 0)); } // non-zero
        inline bool operator !() const { return ((i[0] == 0) && (i[1] == 0)); } // empty
        inline int& operator[](int idx) { return GetPtr()[idx]; }
        inline explicit operator uint2() const { uint2 rtn = { static_cast<unsigned int>(i[0]), static_cast<unsigned int>(i[1]) }; return rtn; }
        inline operator DirectX::XMVECTOR() const { DirectX::XMVECTORI32 r; r.i[0] = i[0]; r.i[1] = i[1]; r.i[2] = r.i[3] = 0; return r.v; }
        inline operator DirectX::XMINT2() const { return Get_XMINT2(); }
        inline operator DirectX::XMFLOAT2() const { return Get_XMFLOAT2(); }
        inline const POINT                      Get_POINT() const { POINT pt = { static_cast<long>(i[0]), static_cast<long>(i[1]) }; return pt; }
        inline const DirectX::XMINT2            Get_XMINT2() const { DirectX::XMINT2 rtn; rtn = { static_cast<int>(i[0]), static_cast<int>(i[1]) }; return rtn; }
        inline const DirectX::XMUINT2           Get_XMUINT2() const { DirectX::XMUINT2 rtn; rtn = { static_cast<unsigned int>(i[0]), static_cast<unsigned int>(i[1]) }; return rtn; }
        inline const DirectX::XMFLOAT2          Get_XMFLOAT2() const { DirectX::XMFLOAT2 rtn; rtn = { static_cast<float>(i[0]), static_cast<float>(i[1]) }; return rtn; }
        // Comparators
        inline bool operator== (const int2& rhs) const noexcept { return i[0] == rhs.i[0] && i[1] == rhs.i[1]; }
        inline bool operator!=  (const int2& rhs) const noexcept { return i[0] != rhs.i[0] || i[1] != rhs.i[1]; }
        inline bool operator< (const int2& rhs) const noexcept { return i[0] < rhs.i[0] && i[1] < rhs.i[1]; }
        inline bool operator> (const int2& rhs) const noexcept { return i[0] > rhs.i[0] && i[1] > rhs.i[1]; }
        inline bool operator<= (const int2& rhs) const noexcept { return i[0] <= rhs.i[0] && i[1] <= rhs.i[1]; }
        inline bool operator>= (const int2& rhs) const noexcept { return i[0] >= rhs.i[0] && i[1] >= rhs.i[1]; }
        // Math Operators
        inline int2 operator- () const { return int2(-1 * i[0], -1 * i[1]); }
        inline int2 operator+ (const int2 p) const { return int2(i[0] + p.i[0], i[1] + p.i[1]); }
        inline int2 operator- (const int2 p) const { return int2(i[0] - p.i[0], i[1] - p.i[1]); }
        inline int2 operator* (const int2 p) const { return int2(i[0] * p.i[0], i[1] * p.i[1]); }
        inline int2 operator/ (const int2 p) const { return int2(i[0] / p.i[0], i[1] / p.i[1]); }

        inline int2& operator+= (const int s) { i[0] += s; i[1] += s; return *this; }
        inline int2& operator-= (const int s) { i[0] = i[0] - s; i[1] = i[1] - s; return *this; }
        inline int2& operator*= (const int s) { i[0] = i[0] * s; i[1] = i[1] * s; return *this; }
        inline int2& operator/= (const int s) { i[0] = i[0] / s; i[1] = i[1] / s; return *this; }

        inline int2& operator+= (const int2 & p) { i[0] = i[0] + p.i[0]; i[1] = i[1] + p.i[1];; return *this; }
        inline int2& operator-= (const int2 & p) { i[0] = i[0] - p.i[0]; i[1] = i[1] - p.i[1];; return *this; }
        inline int2& operator*= (const int2 & p) { i[0] = i[0] * p.i[0]; i[1] = i[1] * p.i[1];; return *this; }
        inline int2& operator/= (const int2 & p) { i[0] = i[0] / p.i[0]; i[1] = i[1] / p.i[1];; return *this; }

        inline int2 operator+  (int s) const { return int2(i[0] + static_cast<long>(s), i[1] + static_cast<long>(s)); }
        inline int2 operator-  (int s) const { return int2(i[0] - static_cast<long>(s), i[1] - static_cast<long>(s)); }
        inline int2 operator*  (int s) const { return int2(i[0] * static_cast<long>(s), i[1] * static_cast<long>(s)); }
        inline int2 operator/  (int s) const { return int2(i[0] / static_cast<long>(s), i[1] / static_cast<long>(s)); }
        
        inline int2 operator+  (long s) const { return int2(i[0] + (s), i[1] + (s)); }
        inline int2 operator-  (long s) const { return int2(i[0] - (s), i[1] - (s)); }
        inline int2 operator*  (long s) const { return int2(i[0] * (s), i[1] * (s)); }
        inline int2 operator/  (long s) const { return int2(i[0] / (s), i[1] / (s)); }

        inline int2 operator+  (float s) const { return int2(static_cast<float>(i[0]) + (s), static_cast<float>(i[1]) + (s)); }
        inline int2 operator-  (float s) const { return int2(static_cast<float>(i[0]) - (s), static_cast<float>(i[1]) - (s)); }
        inline int2 operator*  (float s) const { return int2(static_cast<float>(i[0])* (s), static_cast<float>(i[1])* (s)); }
        inline int2 operator/  (float s) const { return int2(static_cast<float>(i[0]) / (s), static_cast<float>(i[1]) / (s)); }

        inline int2& operator+= (const float s) { i[0] = static_cast<int>(static_cast<float>(i[0]) + s); i[1] = static_cast<int>(static_cast<float>(i[1]) + s); return *this; }
        inline int2& operator-= (const float s) { i[0] = static_cast<int>(static_cast<float>(i[0]) - s); i[1] = static_cast<int>(static_cast<float>(i[1]) - s); return *this; }
        inline int2& operator*= (const float s) { i[0] = static_cast<int>(static_cast<float>(i[0])* s); i[1] = static_cast<int>(static_cast<float>(i[1])* s); return *this; }
        inline int2& operator/= (const float s) { i[0] = static_cast<int>(static_cast<float>(i[0]) / s); i[1] = static_cast<int>(static_cast<float>(i[1]) / s); return *this; }

        // Assignments
        inline void constexpr                   SetZero(void) { i[0] = i[1] = 0; }
        inline void constexpr                   SetX(const long x) { i[0] = static_cast<int>(x); }
        inline void constexpr                   SetY(const long y) { i[1] = static_cast<int>(y); }
        inline void constexpr                   Set(const long xy) { i[0] = i[1] = static_cast<int>(xy); }
        inline void constexpr                   Set(const float xy) { i[0] = i[1] = static_cast<int>(xy); }
        inline void constexpr                   Set(const long x, const long y) { i[0] = x; i[1] = y; }
        inline void constexpr                   Set(const int x, const int y) { i[0] = x; i[1] = y; }
        inline void constexpr                   Set(const unsigned int x, const unsigned int y) { i[0] = static_cast<int>(x); i[1] = static_cast<int>(y); }
        inline void constexpr                   Set(const float x, const float y) { i[0] = static_cast<int>(x); i[1] = static_cast<int>(y); }
        inline void constexpr                   Set(const unsigned long x, const unsigned long y) { i[0] = static_cast<int>(x); i[1] = static_cast<int>(y); }
        inline void                             Set(const int2 & in) { *this = in; }
        inline void                             Set(const uint2 & in) { i[0] = static_cast<int>(in.GetX()); i[1] = static_cast<int>(in.GetY()); }
        inline void                             Set(const DirectX::XMINT2 & point) { i[0] = point.x; i[1] = point.y; }
        inline void                             Set(const POINT & point) { i[0] = static_cast<int>(point.x); i[1] = static_cast<int>(point.y); }
        // Tests
        bool                                    IsZero() const { return (i[0] == 0 && i[1] == 0); }
        // Accessors
        int*                                    GetPtr() { return reinterpret_cast<int*>(this); }
        inline const int                        GetX() const { return (int)i[0]; }
        inline const int                        GetY() const { return (int)i[1]; }
        inline auto                             GetLength() const { return static_cast<int>(std::sqrt(i[0] * i[0] + i[1] * i[1])); } // common naming
        // Functionality
        inline void                             Min(const int2& in) { i[0] = i[0] < in.i[0] ? i[0] : in.i[0]; i[1] = i[1] < in.i[1] ? i[1] : in.i[1]; }
        inline void                             Max(const int2& in) { i[0] = i[0] > in.i[0] ? i[0] : in.i[0]; i[1] = i[1] > in.i[1] ? i[1] : in.i[1]; }
        inline void                             MakeAbsolute() { i[0] = std::abs(i[0]); i[1] = std::abs(i[1]); }
        // Statics
        static const float                      Magnitude(const int2& pIn) { return (float)std::sqrt(pIn.i[0] * pIn.i[0] + pIn.i[1] * pIn.i[1]); }
    };
    /******************************************************************************
    *   int3
    *       Original class not adapted to DirectX intrinsics for acceleration
    *       to keep three signed ints and treat as one data type
    ******************************************************************************/
    class alignas(16) int3
    {
        /* variables */
    public:
        signed int      i[3] = { 0, 0, 0 };
    protected:

    private:

        /* methods */
    public:
        // Creation/Life cycle
#if defined(_MSC_VER)
        void* operator new (std::size_t size) noexcept(false) { auto ptr = ::operator new(size, std::align_val_t{ 16 }); if (!ptr) throw std::bad_alloc(); return ptr; }
        void  operator delete (void* p) noexcept { ::operator delete(p, std::align_val_t{ 16 }); }
#elif defined(__GNUC__) || defined(__clang__)
        void* operator new(std::size_t size) noexcept(false) { void* ptr; if (posix_memalign(&ptr, 16, size) != 0) { throw std::bad_alloc(); } return ptr; }
        void operator delete(void* p) noexcept { free(p); }
#endif
        inline constexpr int3() {}
        inline constexpr int3(const long xyz) { Set(xyz); }
        inline constexpr int3(const long x, const long y, const long z) { Set(x, y, z); }
        inline constexpr int3(const int x, const int y, const int z) { Set(x, y, z); }
        inline constexpr int3(const unsigned int x, const unsigned int y, const unsigned int z) { Set(x, y, z); }
        inline constexpr int3(const float x, const float y, const float z) { Set(x, y, z); }
        inline constexpr int3(const unsigned long x, const unsigned long y, const unsigned long z) { Set(x, y, z); }
        inline int3(const int3 & in) = default; // copy
        inline int3(const float3 & in);
        inline int3(const DirectX::XMINT3 & in) { Set(in); }
        inline int3(int3 && in) = default; // move 
        inline explicit int3(const DirectX::XMVECTOR vecIn) { DirectX::XMINT3 t; DirectX::XMStoreSInt3(&t, vecIn); i[0] = t.x; i[1] = t.y; i[2] = t.z; } // integer vector input NOT float
        virtual ~int3() = default;
        // Operators 
        inline int3& operator= (const int3 & in) = default; // copy assignment
        inline int3& operator= (const DirectX::XMINT3 & in) { Set(in); return *this; } // copy assignment
        inline int3& operator= (const DirectX::XMVECTOR & vecIn) { unsigned int d[3]; auto v = DirectX::XMConvertVectorFloatToInt(vecIn, 0); DirectX::XMStoreInt3(d, v); i[0] = (long)d[0]; i[1] = (long)d[1]; i[2] = (long)d[2]; return *this; } // untested
        inline int3& operator= (int3 && in) = default; // move assignment
        // Conversions
        inline explicit operator bool() const { return (((i[0] != 0) || (i[1] != 0) || (i[2] != 0))  ); } // non-zero
        inline bool operator !() const { return ( ((i[0] == 0) && (i[1] == 0) && (i[2] == 0)) );} // empty
        inline int& operator[](int idx) { return GetPtr()[idx]; }
        inline operator DirectX::XMVECTOR() const { DirectX::XMVECTORI32 r; r.i[0] = i[0]; r.i[1] = i[1]; r.i[2] = i[2]; r.i[3] = 0; return r.v; }
        inline operator DirectX::XMINT3() const { return Get_XMINT3(); }
        inline operator DirectX::XMFLOAT3() const { return Get_XMFLOAT3(); }
        inline const DirectX::XMINT3            Get_XMINT3() const { DirectX::XMINT3 rtn; rtn = { static_cast<int>(i[0]), static_cast<int>(i[1]), static_cast<int>(i[2]) }; return rtn; }
        inline const DirectX::XMUINT3           Get_XMUINT3() const { DirectX::XMUINT3 rtn; rtn = { static_cast<unsigned int>(i[0]), static_cast<unsigned int>(i[1]), static_cast<unsigned int>(i[2]) }; return rtn; }
        inline const DirectX::XMFLOAT3          Get_XMFLOAT3() const { DirectX::XMFLOAT3 rtn; rtn = { static_cast<float>(i[0]), static_cast<float>(i[1]), static_cast<float>(i[2]) }; return rtn; }
        // Comparators
        inline bool operator==  (const int3& rhs) const noexcept { return i[0] == rhs.i[0] && i[1] == rhs.i[1] && i[2] == rhs.i[2]; }
        inline bool operator!=  (const int3& rhs) const noexcept { return i[0] != rhs.i[0] || i[1] != rhs.i[1] || i[2] != rhs.i[2]; }
        inline bool operator< (const int3& rhs) const noexcept { return i[0] < rhs.i[0] && i[1] < rhs.i[1] && i[2] < rhs.i[2]; }
        inline bool operator> (const int3& rhs) const noexcept { return i[0] > rhs.i[0] && i[1] > rhs.i[1] && i[2] > rhs.i[2]; }
        inline bool operator<= (const int3& rhs) const noexcept { return i[0] <= rhs.i[0] && i[1] <= rhs.i[1] && i[2] <= rhs.i[2]; }
        inline bool operator>= (const int3& rhs) const noexcept { return i[0] >= rhs.i[0] && i[1] >= rhs.i[1] && i[2] >= rhs.i[2]; }
        // Math Operators
        inline int3 operator- () const { return int3(-1 * i[0], -1 * i[1], -1 * i[2]); }
        inline int3 operator+ (const int3 p) const { return int3(i[0] + p.i[0], i[1] + p.i[1], i[2] + p.i[2]); }
        inline int3 operator- (const int3 p) const { return int3(i[0] - p.i[0], i[1] - p.i[1], i[2] - p.i[2]); }
        inline int3 operator* (const int3 p) const { return int3(i[0] * p.i[0], i[1] * p.i[1], i[2] * p.i[2]); }
        inline int3 operator/ (const int3 p) const { return int3(i[0] / p.i[0], i[1] / p.i[1], i[2] / p.i[2]); }

        inline int3& operator+= (const int s) { i[0] = i[0] + s; i[1] = i[1] + s; i[2] = i[2] + s; return *this; }
        inline int3& operator-= (const int s) { i[0] = i[0] - s; i[1] = i[1] - s; i[2] = i[2] - s; return *this; }
        inline int3& operator*= (const int s) { i[0] = i[0] * s; i[1] = i[1] * s; i[2] = i[2] * s; return *this; }
        inline int3& operator/= (const int s) { i[0] = i[0] / s; i[1] = i[1] / s; i[2] = i[2] / s; return *this; }

        inline int3& operator+= (const int3 & p) { i[0] = i[0] + p.i[0]; i[1] = i[1] + p.i[1]; i[2] = i[2] + p.i[2]; return *this; }
        inline int3& operator-= (const int3 & p) { i[0] = i[0] - p.i[0]; i[1] = i[1] - p.i[1]; i[2] = i[2] - p.i[2]; return *this; }
        inline int3& operator*= (const int3 & p) { i[0] = i[0] * p.i[0]; i[1] = i[1] * p.i[1]; i[2] = i[2] * p.i[2]; return *this; }
        inline int3& operator/= (const int3 & p) { i[0] = i[0] / p.i[0]; i[1] = i[1] / p.i[1]; i[2] = i[2] / p.i[2]; return *this; }

        inline int3 operator+  (int s) const { return int3(i[0] + static_cast<long>(s), i[1] + static_cast<long>(s), i[2] + static_cast<long>(s)); }
        inline int3 operator-  (int s) const { return int3(i[0] - static_cast<long>(s), i[1] - static_cast<long>(s), i[2] - static_cast<long>(s)); }
        inline int3 operator*  (int s) const { return int3(i[0] * static_cast<long>(s), i[1] * static_cast<long>(s), i[2] * static_cast<long>(s)); }
        inline int3 operator/  (int s) const { return int3(i[0] / static_cast<long>(s), i[1] / static_cast<long>(s), i[2] / static_cast<long>(s)); }

        inline int3 operator+  (long s) const { return int3(i[0] + (s), i[1] + (s), i[2] + (s)); }
        inline int3 operator-  (long s) const { return int3(i[0] - (s), i[1] - (s), i[2] - (s)); }
        inline int3 operator*  (long s) const { return int3(i[0] * (s), i[1] * (s), i[2] * (s)); }
        inline int3 operator/  (long s) const { return int3(i[0] / (s), i[1] / (s), i[2] / (s)); }

        inline int3 operator+  (float s) const { return int3(static_cast<float>(i[0]) + (s), static_cast<float>(i[1]) + (s), static_cast<float>(i[2]) + (s)); }
        inline int3 operator-  (float s) const { return int3(static_cast<float>(i[0]) - (s), static_cast<float>(i[1]) - (s), static_cast<float>(i[2]) - (s)); }
        inline int3 operator*  (float s) const { return int3(static_cast<float>(i[0])* (s), static_cast<float>(i[1])* (s), static_cast<float>(i[2])* (s)); }
        inline int3 operator/  (float s) const { return int3(static_cast<float>(i[0]) / (s), static_cast<float>(i[1]) / (s), static_cast<float>(i[2]) / (s)); }

        inline int3& operator+= (const float s) { i[0] = static_cast<int>(static_cast<float>(i[0]) + s); i[1] = static_cast<int>(static_cast<float>(i[1]) + s); i[2] = static_cast<int>(static_cast<float>(i[2]) + s); return *this; }
        inline int3& operator-= (const float s) { i[0] = static_cast<int>(static_cast<float>(i[0]) - s); i[1] = static_cast<int>(static_cast<float>(i[1]) - s); i[2] = static_cast<int>(static_cast<float>(i[2]) - s); return *this; }
        inline int3& operator*= (const float s) { i[0] = static_cast<int>(static_cast<float>(i[0])* s); i[1] = static_cast<int>(static_cast<float>(i[1])* s); i[2] = static_cast<int>(static_cast<float>(i[2])* s); return *this; }
        inline int3& operator/= (const float s) { i[0] = static_cast<int>(static_cast<float>(i[0]) / s); i[1] = static_cast<int>(static_cast<float>(i[1]) / s); i[2] = static_cast<int>(static_cast<float>(i[2]) / s); return *this; }

        // Assignments
        inline void constexpr                   SetZero(void) { i[0] = i[1] = i[2] = 0; }
        inline void constexpr                   SetX(const long x) { i[0] = static_cast<int>(x); }
        inline void constexpr                   SetY(const long y) { i[1] = static_cast<int>(y); }
        inline void constexpr                   SetZ(const long z) { i[2] = static_cast<int>(z); }
        inline void constexpr                   Set(const long xyz) { i[0] = i[1] = i[2] = static_cast<int>(xyz); }
        inline void constexpr                   Set(const float xyz) { i[0] = i[1] = i[2] = static_cast<int>(xyz); }
        inline void constexpr                   Set(const long x, const long y, const long z) { i[0] = static_cast<int>(x); i[1] = static_cast<int>(y); i[2] = static_cast<int>(z); }
        inline void constexpr                   Set(const int x, const int y, const int z) { i[0] = x; i[1] = y; i[2] = z; }
        inline void constexpr                   Set(const unsigned int x, const unsigned int y, const unsigned int z) { i[0] = static_cast<int>(x); i[1] = static_cast<int>(y); i[2] = static_cast<int>(z); }
        inline void constexpr                   Set(const float x, const float y, const float z) { i[0] = static_cast<int>(x); i[1] = static_cast<int>(y); i[2] = static_cast<int>(z); }
        inline void constexpr                   Set(const unsigned long x, const unsigned long y, const unsigned long z) { i[0] = static_cast<int>(x); i[1] = static_cast<int>(y); i[2] = static_cast<int>(z); }
        inline void                             Set(const int3 & in) { *this = in; }
        inline void                             Set(const DirectX::XMINT3 & point) { i[0] = point.x; i[1] = point.y; i[2] = point.z; }
        // Tests
        bool                                    IsZero() const { return (i[0] == 0 && i[1] == 0 && i[2] == 0); }
        // Accessors
        int*                                    GetPtr() { return reinterpret_cast<int*>(this); }
        inline const int                        GetX() const { return (int)i[0]; }
        inline const int                        GetY() const { return (int)i[1]; }
        inline const int                        GetZ() const { return (int)i[2]; }
        inline auto                             GetLength() const { return static_cast<int>(std::sqrt(i[0] * i[0] + i[1] * i[1] + i[2] * i[2])); } // common naming
        // Functionality
        inline void                             Max(const int3& in) { i[0] = i[0] > in.i[0] ? i[0] : in.i[0]; i[1] = i[1] > in.i[1] ? i[1] : in.i[1]; i[2] = i[2] > in.i[2] ? i[2] : in.i[2]; }
        inline void                             Min(const int3& in) { i[0] = i[0] < in.i[0] ? i[0] : in.i[0]; i[1] = i[1] < in.i[1] ? i[1] : in.i[1]; i[2] = i[2] < in.i[2] ? i[2] : in.i[2]; }
        inline void                             MakeAbsolute() { i[0] = std::abs(i[0]); i[1] = std::abs(i[1]); i[2] = std::abs(i[2]); }
        // Statics
        static const float                      Magnitude(const int3& pIn) { return (float)std::sqrt(pIn.i[0] * pIn.i[0] + pIn.i[1] * pIn.i[1] + pIn.i[2] * pIn.i[2]); }
    };
    /******************************************************************************
    *   float2
    *       Converted to use DirectX intrinsics
    ******************************************************************************/
    class alignas(16) float2 : public DirectX::XMVECTORF32
    {
        /* variables */
    public:

    protected:

    private:

        /* methods */
    public:
        // Creation/Life cycle
        static std::shared_ptr<float2> Create() { return std::make_shared<float2>(); }
        static std::unique_ptr<float2> CreateUnique() { return std::make_unique<float2>(); }
        inline float2() { SetZero(); }
        inline float2(const float xy) { Set(xy); }
        inline float2(const float x, const float y) { Set(x, y); }
        inline float2(const int x, const int y) { Set(x, y); }
        inline float2(const unsigned int x, const unsigned int y) { Set(x, y); }
        inline float2(const long x, const long y) { Set(x, y); }
        inline float2(const unsigned long x, unsigned const long y) { Set(x, y); }
        inline float2(const float2 & in) noexcept { v = in.v; } // copy
        float2(float3 vecIn);
        inline float2(const DirectX::XMFLOAT2 & in) { Set(in); }
        inline float2(const DirectX::XMINT2 & in) { Set(in); }
        inline float2(const int2 & in) { Set(in.Get_XMFLOAT2()); }
        inline float2(const uint2 & in) { Set(in.Get_XMFLOAT2()); }
        inline float2(float2 && in) noexcept { v = std::move(in.v); } // move
        inline float2(const DirectX::XMVECTOR & vecIn) { v = vecIn; }
        inline float2(const DirectX::XMVECTORF32 & vecIn) { v = vecIn.v; }
        inline float2(const std::vector<float> &f) { if (f.size() > 1) v = DirectX::XMVectorSet(f[0], f[1], 0.f, 0.f); }
        inline float2(uint8_t * bytesIn) { auto fp = reinterpret_cast<DirectX::XMFLOAT2*>(bytesIn); v = DirectX::XMLoadFloat2(fp); }
        inline float2(const float* floatIn) { auto l = DirectX::XMFLOAT2(floatIn); v = DirectX::XMLoadFloat2(&l); }
        virtual ~float2() = default;
        // Operators 
        inline float2& operator= (const float2 &in) noexcept { Set(in); return *this; } // copy assignment
        inline float2& operator= (const DirectX::XMFLOAT2 & in) noexcept { Set(in); return *this; } // copy assignment
        inline float2& operator= (const DirectX::XMVECTOR vecIn) noexcept { v = vecIn; return *this; } // copy assignment
        inline float2& operator= (float2 && in) noexcept { v = std::move(in.v); return *this; } // move
        // Conversions
        inline explicit operator bool() const { return !DirectX::XMVector2IsNaN(v) && !DirectX::XMVector2IsInfinite(v); } // valid
        inline bool operator !() const { return DirectX::XMVector2IsNaN(v) || DirectX::XMVector2IsInfinite(v); } // invalid
        inline operator DirectX::XMFLOAT2() const { return Get_XMFLOAT2(); }
        inline operator DirectX::XMFLOAT2A() const { return Get_XMFLOAT2A(); }
        inline operator DirectX::XMINT2() const { return Get_XMINT2(); }
        inline operator DirectX::XMUINT2() const { return Get_XMUINT2(); }
        inline explicit operator DirectX::XMVECTORF32() const { return Get_XMVECTORF32(); }
        inline explicit operator const float* () const { return f; }
        inline explicit operator float* () { return f; }
        inline explicit operator const uint8_t* () const { return reinterpret_cast<const uint8_t*>(f); }
        inline explicit operator uint8_t* () { return reinterpret_cast<uint8_t*>(f); }
        // Comparators
        // directX behavior is not based on magnitudes, instead compares each component.  This differs such that
        //  if (4,4) is compared to (2,25), a < test fails DirectX::XMVector2Less but passes on mag
        //  this is important, since most common use is geometry comparisons (area, vol, etc.), where the data
        //  proposal on 12/31/2020 to revert this back to like our int2 implementation of magnitudes.
        inline bool operator<  (const float2& rhs) const noexcept { return DirectX::XMVector2Less(v, rhs); }
        inline bool operator<= (const float2& rhs) const noexcept { return DirectX::XMVector2LessOrEqual(v, rhs); }
        inline bool operator>  (const float2& rhs) const noexcept { return DirectX::XMVector2Greater(v, rhs); }
        inline bool operator>= (const float2& rhs) const noexcept { return DirectX::XMVector2GreaterOrEqual(v, rhs); }
        inline bool operator== (const float2& rhs) const noexcept { return DirectX::XMVector2Equal(v, rhs); }
        inline bool operator!= (const float2& rhs) const noexcept { return DirectX::XMVector2NotEqual(v, rhs); }
        // Math Operators
        inline float2 operator- () const { return float2(DirectX::XMVectorNegate(v)); }
        inline float2 operator+ (const float2 & p) const { return float2(DirectX::XMVectorAdd(v, p.v)); }
        inline float2 operator- (const float2 & p) const { return float2(DirectX::XMVectorSubtract(v, p.v)); }
        inline float2 operator* (const float2 & p) const { return float2(DirectX::XMVectorMultiply(v, p.v)); }
        inline float2 operator/ (const float2 & p) const { return float2(DirectX::XMVectorDivide(v, p.v)); }
        inline DirectX::XMVECTOR& operator+= (const float2 & p) { return v = DirectX::XMVectorAdd(v, p.v); }
        inline DirectX::XMVECTOR& operator-= (const float2 & p) { return v = DirectX::XMVectorSubtract(v, p.v); }
        inline DirectX::XMVECTOR& operator*= (const float2 & p) { return v = DirectX::XMVectorMultiply(v, p.v); }
        inline DirectX::XMVECTOR& operator/= (const float2 & p) { return v = DirectX::XMVectorDivide(v, p.v); }
        inline float2 operator+ (const DirectX::XMVECTOR vecIn) const { return float2(DirectX::XMVectorAdd(v, vecIn)); }
        inline float2 operator- (const DirectX::XMVECTOR vecIn) const { return float2(DirectX::XMVectorSubtract(v, vecIn)); }
        inline float2 operator* (const DirectX::XMVECTOR vecIn) const { return float2(DirectX::XMVectorMultiply(v, vecIn)); }
        inline float2 operator/ (const DirectX::XMVECTOR vecIn) const { return float2(DirectX::XMVectorDivide(v, vecIn)); }
        inline float2& operator+= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorAdd(v, vecIn); return *this; }
        inline float2& operator-= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorSubtract(v, vecIn); return *this; }
        inline float2& operator*= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorMultiply(v, vecIn); return *this; }
        inline float2& operator/= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorDivide(v, vecIn); return *this; }
        inline float2 operator+ (float s) const { return float2(DirectX::XMVectorAdd(v, float2(s))); }
        inline float2 operator- (float s) const { return float2(DirectX::XMVectorSubtract(v, float2(s))); }
        inline float2 operator* (float s) const { return float2(DirectX::XMVectorMultiply(v, float2(s))); }
        inline float2 operator/ (float s) const { return float2(DirectX::XMVectorDivide(v, float2(s))); }
        inline DirectX::XMVECTOR& operator+= (float s) { return v = DirectX::XMVectorAdd(v, float2(s)); }
        inline DirectX::XMVECTOR& operator-= (float s) { return v = DirectX::XMVectorSubtract(v, float2(s)); }
        inline DirectX::XMVECTOR& operator*= (float s) { return v = DirectX::XMVectorScale(v, s); }
        inline DirectX::XMVECTOR& operator/= (float s) { return v = DirectX::XMVectorDivide(v, float2(s)); }
        inline float2 operator* (const DirectX::XMMATRIX& m) { return DirectX::XMVector2TransformNormal(v, m); } // without translation
        inline float2 operator*= (const DirectX::XMMATRIX& m) { v = DirectX::XMVector2TransformNormal(v, m); return *this; } // without translation
        // Accessors
        float* GetPtr() { return reinterpret_cast<float*>(this); } // returns a float
        unsigned char* GetBytePtr() { return reinterpret_cast<unsigned char*>(this); } // returns a char
        inline const DirectX::XMFLOAT2          Get_XMFLOAT2() const { DirectX::XMFLOAT2 rtn; DirectX::XMStoreFloat2(&rtn, v); return rtn; }
        inline const DirectX::XMFLOAT2A         Get_XMFLOAT2A() const { DirectX::XMFLOAT2A rtn; DirectX::XMStoreFloat2A(&rtn, v); return rtn; }
        inline const DirectX::XMINT2            Get_XMINT2() const { DirectX::XMINT2 rtn; rtn.x = static_cast<int>(f[0]); rtn.y = static_cast<int>(f[1]); return rtn; }
        inline const DirectX::XMUINT2           Get_XMUINT2() const { DirectX::XMUINT2 rtn; rtn.x = static_cast<unsigned int>(f[0]); rtn.y = static_cast<unsigned int>(f[1]); return rtn; }
        inline const DirectX::XMVECTORF32       Get_XMVECTORF32() const { return static_cast<const DirectX::XMVECTORF32>(*this); }
        inline const float                      GetX() const { return (float)DirectX::XMVectorGetX(v); }
        inline const float                      GetY() const { return (float)DirectX::XMVectorGetY(v); }
        inline DirectX::XMVECTOR&               GetVec() { return v; } // modifiable type
        inline const DirectX::XMVECTOR&         GetVecConst() const { return v; } // constant type

        float virtual                           GetMagnitude() const { return DirectX::XMVectorGetX(DirectX::XMVector2Length(v)); }
        float virtual                           GetMagnitudeEst() const { return DirectX::XMVectorGetX(DirectX::XMVector2LengthEst(v)); }
        float virtual                           GetLength() const { return DirectX::XMVectorGetX(DirectX::XMVector2Length(v)); } // common naming
        // Assignments
        inline void __vectorcall                Set(float2 in) noexcept { v = in.v; }
        inline void __vectorcall                Set(DirectX::XMVECTOR in) noexcept { v = in; }
        inline void                             SetZero(void) { v = DirectX::XMVectorZero(); }
        inline void                             SetZeroIfNear(const float epsilon = 0.00005f) { auto mask = DirectX::XMVectorLess(DirectX::XMVectorAbs(v), DirectX::XMVectorReplicate(epsilon)); v = DirectX::XMVectorSelect(v, DirectX::XMVectorZero(), mask); }
        void                                    SetX(const float x) { v = DirectX::XMVectorSetX(v, x); }
        void                                    SetY(const float y) { v = DirectX::XMVectorSetY(v, y); }
        inline virtual void                     Set(const float xy) { v = DirectX::XMVectorSet(xy, xy, 0.f, 0.f); }
        inline void                             Set(const float x, const float y) { v = DirectX::XMVectorSet(x, y, 0.f, 0.f); }
        inline void                             Set(const int x, const int y) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), 0.f, 0.f); }
        inline void                             Set(const unsigned int x, const unsigned int y) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), 0.f, 0.f); }
        inline void                             Set(const long x, const long y) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), 0.f, 0.f); }
        inline void                             Set(const unsigned long x, const unsigned long y) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), 0.f, 0.f); }
        inline void                             Set(const int2 & in) { Set(in.Get_XMFLOAT2()); }
        inline void                             Set(const uint2 & in) { Set(in.Get_XMFLOAT2()); }
        inline void                             Set(const DirectX::XMFLOAT2 & point) { v = DirectX::XMLoadFloat2(&point); }
        inline void                             Set(const DirectX::XMUINT2 & point) { v = DirectX::XMLoadUInt2(&point); v = DirectX::XMConvertVectorUIntToFloat(v, 0); }
        inline void                             Set(const DirectX::XMINT2 & point) { v = DirectX::XMLoadSInt2(&point); v = DirectX::XMConvertVectorIntToFloat(v, 0); }
        // Tests
        bool                                    IsZero() const { return DirectX::XMVector2Equal(v, DirectX::g_XMZero); }
        bool                                    IsZeroOrNearZero(const float epsilon = 0.00005f) const { return DirectX::XMVector2NearEqual(v, DirectX::XMVectorZero(), DirectX::XMVectorReplicate(epsilon)); }
        bool                                    IsValueOrNearValue(float2 value, const float epsilon = 0.00005f) const { return DirectX::XMVector2NearEqual(v, value.GetVec(), DirectX::XMVectorReplicate(epsilon)); }
        // Functionality
        inline float __vectorcall               DotProduct(const float2 vecIn) const { auto d = (float)DirectX::XMVectorGetX(DirectX::XMVector2Dot(v, vecIn)); assert(!isnan(d)); return d; } // order does not mater A•B = B•A
        inline float2 __vectorcall              CrossProduct(const float2 vecIn) const { return float2(DirectX::XMVector3Cross(v, vecIn)); } // order does matter AxB = -(BxA) // note: this is RHS used by DirectX (verified math on 3/5/2022 CHK)
        float2 __vectorcall                     ProjectOnToVector(const float2 vecIn) const { auto n = Normal(vecIn); if (DirectX::XMVector2IsNaN(n)) return float2(0.f); return n * DirectX::XMVector2Dot(v, n.GetVecConst()); }
        inline virtual void                     Absolute() { v = DirectX::XMVectorAbs(v); }
        inline virtual void                     Normalize() { v = DirectX::XMVector2Normalize(v); } // alternate naming (many prefer, future will depreciate one)
        inline virtual void                     MakeNormalize() { v = DirectX::XMVector2Normalize(v); }
        // Statics
        static float2 __vectorcall              Normal(const float2 point2In) { return float2(DirectX::XMVector2Normalize(point2In)); }
        static const float __vectorcall         Magnitude(const float2 point2In) { return DirectX::XMVectorGetX(DirectX::XMVector2Length(point2In)); }
        static const float __vectorcall         MagnitudeEst(const float2 point2In) { return DirectX::XMVectorGetX(DirectX::XMVector2LengthEst(point2In)); }
        static float2 __vectorcall              DotProduct(const float2 vec1In, const float2 vec2In) { return DirectX::XMVector2Dot(vec1In, vec2In); } // order does not mater A•B = B•A
        static float2 __vectorcall              CrossProduct(const float2 vec1In, const float2 vec2In) { return DirectX::XMVector2Cross(vec1In, vec2In); } // order does mater AxB = -(BxA)
        static float __vectorcall               SumComponents(const float2 vec1In) { return DirectX::XMVectorGetX(DirectX::XMVectorSum(vec1In)); }
        static float2 __vectorcall              MultiplyAdd(const float2 vec1MulIn, const float2 & vec2MulIn, const float2 & vec3AddIn) { return DirectX::XMVectorMultiplyAdd(vec1MulIn, vec2MulIn, vec3AddIn); }
        static float2                           Average(const std::vector<float2> & arrayIn) { assert(arrayIn.size()); float2 ave; for (const auto& each : arrayIn) ave += each; ave /= (float)arrayIn.size(); return ave; }
    };
    /******************************************************************************
    *   float3
    *       Converted to use DirectX intrinsics
    ******************************************************************************/
    class alignas(16) float3 : public float2
    {
        /* variables */
    public:

    protected:

    private:

        /* methods */
    public:
        // Creation/Life cycle
        static std::shared_ptr<float3> Create() { return std::make_shared<float3>(); }
        static std::unique_ptr<float3> CreateUnique() { return std::make_unique<float3>(); }
        inline float3() = default; // { SetZero(); }
        inline float3(float xyz) { Set(xyz); }
        inline float3(float x, float y, float z) { Set(x, y, z); }
        inline float3(const float2 & in, const float& zIn) { v = float2(in); SetZ(zIn); }
        inline float3(const float3 & in) noexcept { v = in.v; } // copy
        inline float3(const int3 & in) { Set(in.Get_XMFLOAT3()); }
        inline float3(const DirectX::XMFLOAT3 & in) { Set(in); }
        inline float3(float3 && in) noexcept { v = std::move(in.v); } // move
        inline float3(const DirectX::XMVECTOR vecIn) { v = DirectX::XMVectorSetW(vecIn, 0.f); }
        inline float3(const DirectX::XMVECTORF32 vecIn) noexcept { v = vecIn.v; }
        float3(float4 vecIn);
        inline float3(const std::vector<float>& f) { v = DirectX::XMVectorSet(f[0], f[1], f[2], 0.f); }
        inline float3(uint8_t * bytesIn) { auto l = DirectX::XMFLOAT3(reinterpret_cast<float*>(bytesIn)); v = DirectX::XMLoadFloat3(&l); }
        inline float3(const float* floatIn) { auto l = DirectX::XMFLOAT3(floatIn); v = DirectX::XMLoadFloat3(&l); }
        virtual ~float3() = default;
        // Operators 
        inline float3& operator= (const float3 & in) = default; // copy assignment
        inline float3& operator= (const DirectX::XMFLOAT3 & in) noexcept { Set(in); return *this; } // copy assignment
        inline float3& operator= (const DirectX::XMVECTOR & vecIn) noexcept { v = vecIn; return *this; }
        inline float3& operator= (float3 && in) noexcept { v = std::move(in.v); return *this; }
        // Conversions
        inline explicit operator bool() const { return !DirectX::XMVector3IsNaN(v) && !DirectX::XMVector3IsInfinite(v); } // valid
        inline bool operator !() const { return DirectX::XMVector3IsNaN(v) || DirectX::XMVector3IsInfinite(v); } // invalid
        inline operator float2() const { return float2(v); }
        inline operator DirectX::XMFLOAT3() const { return Get_XMFLOAT3(); }
        inline operator DirectX::XMFLOAT3A() const { return Get_XMFLOAT3A(); }
        inline explicit operator const float* () const { return f; }
        inline explicit operator float* () { return f; }
        inline explicit operator const uint8_t* () const { return reinterpret_cast<const uint8_t*>(f); }
        inline explicit operator uint8_t* () { return reinterpret_cast<uint8_t*>(f); }
        // Comparators
        inline bool operator<  (const float3& rhs) const noexcept { return DirectX::XMVector3Less(v, rhs.GetVecConst()); }
        inline bool operator<= (const float3& rhs) const noexcept { return DirectX::XMVector3LessOrEqual(v, rhs.GetVecConst()); }
        inline bool operator>  (const float3& rhs) const noexcept { return DirectX::XMVector3Greater(v, rhs.GetVecConst()); }
        inline bool operator>= (const float3& rhs) const noexcept { return DirectX::XMVector3GreaterOrEqual(v, rhs.GetVecConst()); }
        inline bool operator== (const float3& rhs) const noexcept { return DirectX::XMVector3Equal(v, rhs.GetVecConst()); }
        inline bool operator!= (const float3& rhs) const noexcept { return DirectX::XMVector3NotEqual(v, rhs.GetVecConst()); }
        // Math Operators
        inline float3 operator- () const { return float3(DirectX::XMVectorNegate(v)); }
        inline float3 operator+ (const float3 s) const { return float3(DirectX::XMVectorAdd(v, s.v)); }
        inline float3 operator- (const float3 s) const { return float3(DirectX::XMVectorSubtract(v, s.v)); }
        inline float3 operator* (const float3 s) const { return float3(DirectX::XMVectorMultiply(v, s.v)); }
        inline float3 operator/ (const float3 s) const { return float3(DirectX::XMVectorDivide(v, s.v)); }
        inline DirectX::XMVECTOR& operator+= (const float3 & s) { return v = DirectX::XMVectorAdd(v, s.v); }
        inline DirectX::XMVECTOR& operator-= (const float3 & s) { return v = DirectX::XMVectorSubtract(v, s.v); }
        inline DirectX::XMVECTOR& operator*= (const float3 & s) { return v = DirectX::XMVectorMultiply(v, s.v); }
        inline DirectX::XMVECTOR& operator/= (const float3 & s) { return v = DirectX::XMVectorDivide(v, s.v); }
        inline float3 operator+ (const DirectX::XMVECTOR vecIn) const { return float3(DirectX::XMVectorAdd(v, vecIn)); }
        inline float3 operator- (const DirectX::XMVECTOR vecIn) const { return float3(DirectX::XMVectorSubtract(v, vecIn)); }
        inline float3 operator* (const DirectX::XMVECTOR vecIn) const { return float3(DirectX::XMVectorMultiply(v, vecIn)); }
        inline float3 operator/ (const DirectX::XMVECTOR vecIn) const { return float3(DirectX::XMVectorDivide(v, vecIn)); }
        inline float3& operator+= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorAdd(v, vecIn); return *this; }
        inline float3& operator-= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorSubtract(v, vecIn); return *this; }
        inline float3& operator*= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorMultiply(v, vecIn); return *this; }
        inline float3& operator/= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorDivide(v, vecIn); return *this; }
        inline float3 operator* (const DirectX::XMMATRIX& m) { return DirectX::XMVector3TransformNormal(v, m); } // without translation
        inline float3& operator*= (const DirectX::XMMATRIX& m) { v = DirectX::XMVector3TransformNormal(v, m); return *this; } // without translation
        inline float3 operator+ (float s) const { DirectX::XMVECTOR sv = _mm_set_ps1(s); return float3(DirectX::XMVectorAdd(v, sv)); }
        inline float3 operator- (float s) const { DirectX::XMVECTOR sv = _mm_set_ps1(s); return float3(DirectX::XMVectorSubtract(v, sv)); }
        inline float3 operator* (float s) const { return float3(DirectX::XMVectorScale(v, s)); }
        inline float3 operator/ (float s) const { DirectX::XMVECTOR sv = _mm_set_ps1(s); return float3(DirectX::XMVectorDivide(v, sv)); }
        inline DirectX::XMVECTOR& operator+= (float s) { DirectX::XMVECTOR sv = _mm_set_ps1(s); return v = DirectX::XMVectorAdd(v, sv); }
        inline DirectX::XMVECTOR& operator-= (float s) { DirectX::XMVECTOR sv = _mm_set_ps1(s); return v = DirectX::XMVectorSubtract(v, sv); }
        inline DirectX::XMVECTOR& operator*= (float s) { return v = DirectX::XMVectorScale(v, s); }
        // Accessors
        inline const DirectX::XMFLOAT3          Get_XMFLOAT3() const { DirectX::XMFLOAT3 rtn; DirectX::XMStoreFloat3(&rtn, v); return rtn; }
        inline const DirectX::XMFLOAT3A         Get_XMFLOAT3A() const { DirectX::XMFLOAT3A rtn; DirectX::XMStoreFloat3A(&rtn, v); return rtn; }
        inline const float                      GetZ() const { return (float)DirectX::XMVectorGetZ(v); }
        inline const float2                     GetXZ() const { return float2((float)DirectX::XMVectorGetX(v), (float)DirectX::XMVectorGetZ(v)); }
        inline const float2                     GetYZ() const { return float2((float)DirectX::XMVectorGetY(v), (float)DirectX::XMVectorGetZ(v)); }
        inline const float2                     GetXY() const { return float2((float)DirectX::XMVectorGetX(v), (float)DirectX::XMVectorGetY(v)); }
        float virtual                           GetMagnitude() const { return DirectX::XMVectorGetX(DirectX::XMVector3Length(v)); }
        float virtual                           GetMagnitudeEst() const { return DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(v)); }
        float virtual                           GetLength() const { return DirectX::XMVectorGetX(DirectX::XMVector3Length(v)); } // common naming
        // Assignments
        inline void                             SetZ(const float z) { v = DirectX::XMVectorSetZ(v, z); }

        inline virtual void                     Set(const float xyz) { v = DirectX::XMVectorSet(xyz, xyz, xyz, 0.f); }
        inline void                             Set(const float x, const float y, const float z) { v = DirectX::XMVectorSet(x, y, z, 0.f); }
        inline void                             Set(const int x, const int y, const int z) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.f); }
        inline void                             Set(const unsigned int x, const unsigned int y, const unsigned int z) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.f); }
        inline void                             Set(const long x, const long y, const long z) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.f); }
        inline void                             Set(const unsigned long x, const unsigned long y, const unsigned long z) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.f); }

        inline void                             Set(const DirectX::XMFLOAT3 & point) { v = DirectX::XMLoadFloat3(&point); }
        inline void                             Set(const DirectX::XMUINT3 & point) { v = DirectX::XMLoadUInt3(&point); v = DirectX::XMConvertVectorUIntToFloat(v, 0); }
        inline void                             Set(const DirectX::XMINT3 & point) { v = DirectX::XMLoadSInt3(&point); v = DirectX::XMConvertVectorIntToFloat(v, 0); }
        // Tests
        // Tests
        bool                                    IsZero() const { return DirectX::XMVector2Equal(v, DirectX::g_XMZero); }
        bool                                    IsOrNearZero(const float epsilon = 0.00005f) const { return DirectX::XMVector3NearEqual(v, DirectX::XMVectorZero(), DirectX::XMVectorReplicate(epsilon)); }
        // Functionality
        inline float __vectorcall               DotProduct(const float3 vecIn) const { auto d = (float)DirectX::XMVectorGetX(DirectX::XMVector3Dot(v, vecIn)); assert(!isnan(d)); return d; } // order does not mater A•B = B•A
        inline float3 __vectorcall              CrossProduct(const float3 vecIn) const { return float3(DirectX::XMVector3Cross(v, vecIn)); } // order does matter AxB = -(BxA) // note: this is RHS used by DirectX (verified math on 3/5/2022 CHK)
        float3 __vectorcall                     ProjectOnToVector(const float3 vecIn) const { auto n = Normal(vecIn); if (DirectX::XMVector3IsNaN(n)) return float3(0.f); return n * DirectX::XMVector3Dot(v, n.GetVecConst()); }
        inline void                             Zero() { v = DirectX::g_XMZero; }
        inline virtual void                     Absolute() { v = DirectX::XMVectorAbs(v); }
        inline virtual void                     Normalize() { v = DirectX::XMVector3Normalize(v); } // alternate naming (many prefer, future will depreciate one)
        inline virtual void                     MakeNormalize() { v = DirectX::XMVector3Normalize(v); }

        // Statics
        static float3 __vectorcall              Normal(const float3 point3In) { return float3(DirectX::XMVector3Normalize(point3In.GetVecConst())); }
        static const float __vectorcall         Magnitude(const float3 point3In) { return DirectX::XMVectorGetX(DirectX::XMVector3Length(point3In.GetVecConst())); }
        static const float __vectorcall         MagnitudeEst(const float3 point3In) { return DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(point3In.GetVecConst())); }
        static float3 __vectorcall              DotProduct(const float3 vec1In, const float3 vec2In) { return DirectX::XMVector3Dot(vec1In, vec2In); } // order does not mater A•B = B•A
        static float3 __vectorcall              CrossProduct(const float3 vec1In, const float3 vec2In) { return float3( DirectX::XMVector3Cross(vec1In, vec2In)); } // order does mater AxB = -(BxA) // note: this is LHS for DirectX, swap the terms for RHS
        static float __vectorcall               SumComponents(const float3 vec1In) { return DirectX::XMVectorGetX(DirectX::XMVectorSum(vec1In)); }
        static float3 __vectorcall              MultiplyAdd(const float3 vec1MulIn, const float3 vec2MulIn, const float3 vec3AddIn) { return DirectX::XMVectorMultiplyAdd(vec1MulIn, vec2MulIn, vec3AddIn); }
        static float3                           Average(const std::vector<float3> & arrayIn) { assert(arrayIn.size()); float3 ave; for (const auto& each : arrayIn) ave += each; ave /= (float)arrayIn.size(); return ave; }
    };
    /******************************************************************************
    *   float4
    *       Converted to use DirectX intrinsics
    ******************************************************************************/
    class alignas(16) float4 : public float3
    {
        /* variables */
    public:

    protected:

    private:

        /* methods */
    public:
        // Creation/Life cycle
        static std::shared_ptr<float4> Create() { return std::make_shared<float4>(); }
        static std::unique_ptr<float4> CreateUnique() { return std::make_unique<float4>(); }
        inline float4() = default;// { SetZero(); }
        inline float4(float xyzw) { Set(xyzw); }
        inline float4(float2 in, float z, float w) { Set(in.GetX(), in.GetY(), z, w); }
        inline float4(float3 in, float w) { Set(in, w); }
        inline float4(float x, float y, float z, float w) { Set(x, y, z, w); }
        inline float4(int2 in, int z = 0, int w = 0) { Set(in, z, w); }
        inline float4(int3 in, int w = 0) { Set(in.GetX(), in.GetY(), in.GetZ(), w); }
        inline float4(uint2 in, unsigned int z = 0, unsigned int w = 0) { Set(in, z, w); }
        inline float4(const float4 & in) noexcept { v = in.v; } // copy
        inline float4(const DirectX::XMFLOAT4 & in) { Set(in); }
        inline float4(float4 && in) noexcept { v = std::move(in.v); } // move
        inline float4(const DirectX::FXMVECTOR & vecIn) { v = vecIn; }
        inline float4(const DirectX::XMVECTORF32 & vecIn) { v = vecIn.v; }
        inline float4(const std::vector<float>& f) { if (f.size() > 3) v = DirectX::XMVectorSet(f[0], f[1], f[2], f[3]); }
        inline float4(uint8_t * bytesIn) { auto fp = reinterpret_cast<DirectX::XMFLOAT4*>(bytesIn); v = DirectX::XMLoadFloat4(fp); }
        inline float4(const float* floatIn) { auto l = DirectX::XMFLOAT4(floatIn); v = DirectX::XMLoadFloat4(&l); }
        virtual ~float4() = default;
        // Operators 
        inline float4& operator= (const float4 & in) = default; // copy assignment
        inline float4& operator= (const DirectX::XMFLOAT4 & in) noexcept { Set(in); return *this; } // copy assignment
        inline float4& operator= (const DirectX::XMVECTOR & vecIn) noexcept { v = vecIn; return *this; }
        inline float4& operator= (float4&& in) noexcept { v = std::move(in.v); return *this; } // move assignment
        // Conversions
        inline explicit operator bool() const { return !DirectX::XMVector4IsNaN(v) && !DirectX::XMVector4IsInfinite(v); } // valid 
        inline bool operator !() const { return DirectX::XMVector4IsNaN(v) || DirectX::XMVector4IsInfinite(v); } // invalid
        inline operator float2() const { return float2(v); }
        inline operator float3() const { return float3(v); }
        inline operator DirectX::XMFLOAT2() const { return Get_XMFLOAT2(); }
        inline operator DirectX::XMFLOAT3() const { return Get_XMFLOAT3(); }
        inline operator DirectX::XMFLOAT4() const { return Get_XMFLOAT4(); }
        inline operator DirectX::XMFLOAT4A() const { return Get_XMFLOAT4A(); }
        inline explicit operator DirectX::XMVECTORF32() const { XMVECTORF32 fv; fv.f[0] = GetX(); fv.f[1] = GetY(); fv.f[2] = GetZ(); fv.f[3] = GetW(); return fv; }
        inline explicit operator const float* () const { return f; }
        inline explicit operator float* () { return f; }
        inline explicit operator const uint8_t* () const { return reinterpret_cast<const uint8_t*>(f); }
        inline explicit operator uint8_t* () { return reinterpret_cast<uint8_t*>(f); }
        // Comparators
        inline bool operator<  (const float4& rhs) const noexcept { return DirectX::XMVector4Less(v, rhs.GetVecConst()); }
        inline bool operator<= (const float4& rhs) const noexcept { return DirectX::XMVector4LessOrEqual(v, rhs.GetVecConst()); }
        inline bool operator>  (const float4& rhs) const noexcept { return DirectX::XMVector4Greater(v, rhs.GetVecConst()); }
        inline bool operator>= (const float4& rhs) const noexcept { return DirectX::XMVector4GreaterOrEqual(v, rhs.GetVecConst()); }
        inline bool operator== (const float4& rhs) const noexcept { return DirectX::XMVector4Equal(v, rhs.GetVecConst()); }
        inline bool operator!= (const float4& rhs) const noexcept { return DirectX::XMVector4NotEqual(v, rhs.GetVecConst()); }
        // Math Operators
        inline float4 operator- () const { return float4(DirectX::XMVectorNegate(v)); }
        inline float4 operator+ (const float4 s) const { return float4(DirectX::XMVectorAdd(v, s.v)); }
        inline float4 operator- (const float4 s) const { return float4(DirectX::XMVectorSubtract(v, s.v)); }
        inline float4 operator* (const float4 s) const { return float4(DirectX::XMVectorMultiply(v, s.v)); }
        inline float4 operator/ (const float4 s) const { return float4(DirectX::XMVectorDivide(v, s.v)); }

        inline float4 operator+ (const float4 & s) { return float4(DirectX::XMVectorAdd(v, s.v)); }
        inline float4 operator- (const float4 & s) { return float4(DirectX::XMVectorSubtract(v, s.v)); }
        inline float4 operator* (const float4 & s) { return float4(DirectX::XMVectorMultiply(v, s.v)); }
        inline float4 operator/ (const float4 & s) { return float4(DirectX::XMVectorDivide(v, s.v)); }
        inline DirectX::XMVECTOR& operator+= (const float4 & s) { return v = DirectX::XMVectorAdd(v, s.v); }
        inline DirectX::XMVECTOR& operator-= (const float4 & s) { return v = DirectX::XMVectorSubtract(v, s.v); }
        inline DirectX::XMVECTOR& operator*= (const float4 & s) { return v = DirectX::XMVectorMultiply(v, s.v); }
        inline DirectX::XMVECTOR& operator/= (const float4 & s) { return v = DirectX::XMVectorDivide(v, s.v); }
        inline float4 operator+ (const DirectX::XMVECTOR & vecIn) { return float4(DirectX::XMVectorAdd(v, vecIn)); }
        inline float4 operator- (const DirectX::XMVECTOR & vecIn) { return float4(DirectX::XMVectorSubtract(v, vecIn)); }
        inline float4 operator* (const DirectX::XMVECTOR & vecIn) { return float4(DirectX::XMVectorMultiply(v, vecIn)); }
        inline float4 operator/ (const DirectX::XMVECTOR & vecIn) { return float4(DirectX::XMVectorDivide(v, vecIn)); }
        inline float4& operator+= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorAdd(v, vecIn); return *this; }
        inline float4& operator-= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorSubtract(v, vecIn); return *this; }
        inline float4& operator*= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorMultiply(v, vecIn); return *this; }
        inline float4& operator/= (const DirectX::XMVECTOR & vecIn) { v = DirectX::XMVectorDivide(v, vecIn); return *this; }
        inline float4 operator+ (float s) { return float4(DirectX::XMVectorAdd(v, DirectX::XMVectorReplicate(s))); }
        inline float4 operator- (float s) { return float4(DirectX::XMVectorSubtract(v, DirectX::XMVectorReplicate(s))); }
        inline float4 operator* (float s) { return float4(DirectX::XMVectorScale(v, s)); }
        inline float4 operator/ (float s) { return float4(DirectX::XMVectorDivide(v, DirectX::XMVectorReplicate(s))); }
        inline DirectX::XMVECTOR& operator+= (float s) { return v = DirectX::XMVectorAdd(v, DirectX::XMVectorReplicate(s)); }
        inline DirectX::XMVECTOR& operator-= (float s) { return v = DirectX::XMVectorSubtract(v, DirectX::XMVectorReplicate(s)); }
        inline DirectX::XMVECTOR& operator*= (float s) { return v = DirectX::XMVectorScale(v, s); }
        inline DirectX::XMVECTOR& operator/= (float s) { return v = DirectX::XMVectorDivide(v, DirectX::XMVectorReplicate(s)); }
        inline float4 operator* (const DirectX::XMMATRIX & m) { return float4(DirectX::XMVector4Transform(v, m)); }
        inline DirectX::XMVECTOR& operator*=  (const DirectX::XMMATRIX & m) { return v = DirectX::XMVector4Transform(v, m); }
        // Accessors
        inline const DirectX::XMFLOAT4          Get_XMFLOAT4() const { DirectX::XMFLOAT4 rtn; DirectX::XMStoreFloat4(&rtn, v); return rtn; }
        inline const DirectX::XMFLOAT4A         Get_XMFLOAT4A() const { DirectX::XMFLOAT4A rtn; DirectX::XMStoreFloat4A(&rtn, v); return rtn; }
        inline const float                      GetW() const { return (float)DirectX::XMVectorGetW(v); }
        float virtual                           GetMagnitude() const { return DirectX::XMVectorGetX(DirectX::XMVector4Length(v)); }
        float virtual                           GetLength() const { return DirectX::XMVectorGetX(DirectX::XMVector4Length(v)); } // common naming
        // Assignments
        inline virtual void                     SetW(const float w) { v = DirectX::XMVectorSetW(v, w); }

        inline virtual void                     Set(const float xyzw) { v = DirectX::XMVectorReplicate(xyzw); }
        inline void                             Set(float3 in, float w) { v = DirectX::XMVectorSetW(in, w); }
        inline void                             Set(const float x, const float y, const float z, const float w) { v = DirectX::XMVectorSet(x, y, z, w); }
        inline void                             Set(const int x, const int y, const int z, const int w) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)); }
        inline void                             Set(int2 in, const signed int z, const signed int w) { v = DirectX::XMVectorSet(static_cast<float>(in.GetX()), static_cast<float>(in.GetY()), static_cast<float>(z), static_cast<float>(w)); }
        inline void                             Set(uint2 in, const unsigned int z, const unsigned int w) { v = DirectX::XMVectorSet(static_cast<float>(in.GetX()), static_cast<float>(in.GetY()), static_cast<float>(z), static_cast<float>(w)); }
        inline void                             Set(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)); }
        inline void                             Set(const long x, const long y, const long z, const long w) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)); }
        inline void                             Set(const unsigned long x, const unsigned long y, const unsigned long z, const unsigned long w) { v = DirectX::XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)); }

        inline void                             Set(const DirectX::XMFLOAT4 & point) { v = DirectX::XMLoadFloat4(&point); }
        inline void                             Set(const DirectX::XMUINT4 & point) { v = DirectX::XMLoadUInt4(&point); v = DirectX::XMConvertVectorUIntToFloat(v, 0); }
        inline void                             Set(const DirectX::XMINT4 & point) { v = DirectX::XMLoadSInt4(&point); v = DirectX::XMConvertVectorIntToFloat(v, 0); }
        // Tests
        bool                                    IsZero() const { return DirectX::XMVector2Equal(v, DirectX::g_XMZero); }
        bool                                    IsOrNearZero(const float epsilon = 0.00005f) const { return DirectX::XMVector4NearEqual(v, DirectX::XMVectorZero(), DirectX::XMVectorReplicate(epsilon)); }
        // Functionality
        inline float __vectorcall               DotProduct(const float4 vecIn) const { auto d = (float)DirectX::XMVectorGetX(DirectX::XMVector4Dot(v, vecIn)); assert(!isnan(d)); return d; } // order does not mater A•B = B•A
        inline float4 __vectorcall              CrossProduct(const float4 vec1In, const float4 vec2In) const { return float4(DirectX::XMVector4Cross(v, vec1In, vec2In)); } // order does matter AxB = -(BxA) // note: this is RHS used by DirectX (verified math on 3/5/2022 CHK)
        float4 __vectorcall                     ProjectOnToVector(const float4 vecIn) const { auto n = Normal(vecIn); if (DirectX::XMVector4IsNaN(n)) return float4(0.f); return n * DirectX::XMVector4Dot(v, n.GetVecConst()); }
        inline void                             Zero() { v = DirectX::g_XMZero; }
        inline virtual void                     Absolute() { v = DirectX::XMVectorAbs(v); }
        inline virtual void                     Normalize() { v = DirectX::XMVector4Normalize(v); } // alternate naming (many prefer, future will depreciate one)
        inline virtual void                     MakeNormalize() { v = DirectX::XMVector4Normalize(v); }
        // Statics
        static float4 __vectorcall              Normal(const float4 point4In) { return float4(DirectX::XMVector4Normalize(point4In.GetVecConst())); }
        static const float __vectorcall         Magnitude(const float4 point4In) { return DirectX::XMVectorGetX(DirectX::XMVector4Length(point4In.GetVecConst())); }
        static float4 __vectorcall              DotProduct(const float4 vec1In, const float4 & vec2In) { return DirectX::XMVector4Dot(vec1In, vec2In); } // order does not mater A•B = B•A
        static float4 __vectorcall              CrossProduct(const float4 vec1In, const float4 & vec2In, const float4 & vec3In) { return DirectX::XMVector4Cross(vec1In, vec2In, vec3In); } // order does mater AxB = -(BxA) // note: this is LHS for DirectX, swap the terms for RHS
        static float __vectorcall               SumComponents(const float4 vec1In) { return DirectX::XMVectorGetX(DirectX::XMVectorSum(vec1In)); }
        static float4 __vectorcall              MultiplyAdd(const float4 vec1MulIn, const float4 & vec2MulIn, const float4 & vec3AddIn) { return DirectX::XMVectorMultiplyAdd(vec1MulIn, vec2MulIn, vec3AddIn); }
        static float4                           Average(const std::vector<float4> arrayIn) { assert(arrayIn.size()); float4 ave; for (const auto& each : arrayIn) ave += each; ave /= (float)arrayIn.size(); return ave; }

    };
    /******************************************************************************
    *   Quaternion
    *
    *   w + x*i + y*j + z*k
    *   based on the priciple that: i^2 = j^2 = k^2 = ijk = -1
    *   then:
    *       w^2-x^2-y^2-z^2=-1,
    *       2wx=0
    *       2wy=0
    *       2wz=0
    *   geometrically, they are then out of phase by 90 degrees (pi/2 radians)
    *   so cross product.
    *   encode the float4 with:
    *       w = cos(angle/2)
    *       x = i * sin(angle/2)
    *       y = j * sin(angle/2)
    *       z = k * sin(angle/2)
    *   where angle is in radians
    *   where i, j, and k are the directions of the unit vector defining the axis
    *   then, the product Q2*Q1 (a rotation Q1 followed by the rotation Q2)
    *   w = Q2.w * Q1.w - Q2.x * Q1.x - Q2.y * Q1.y - Q2.z * Q1.z
    *   x = Q2.w * Q1.x + Q2.x * Q1.w + Q2.y * Q1.z - Q2.z * Q1.y
    *   y = Q2.w * Q1.y - Q2.x * Q1.z + Q2.y * Q1.w + Q2.z * Q1.x
    *   z = Q2.w * Q1.z + Q2.x * Q1.y - Q2.y * Q1.x + Q2.z * Q1.w
    *   https://en.wikipedia.org/wiki/Quaternion
    *       Converted to use DirectX intrinsics
    ******************************************************************************/
    class alignas(16) Quaternion : public float4
    {
    public:
        // Construction/Destruction
        inline explicit Quaternion() noexcept { v = DirectX::XMQuaternionIdentity(); }
        inline explicit Quaternion(const float3 axis) { SetAxisAngle(axis, 0.f); }
        inline explicit Quaternion(const float3 axis, const float angle) { SetAxisAngle(axis, angle); } // [0, +π] radians
        inline explicit Quaternion(const DirectX::XMFLOAT3 & pitchYawRoll) { v = DirectX::XMQuaternionRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z); }
        inline explicit Quaternion(float pitch, float yaw, float roll) { v = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll); }
        inline explicit Quaternion(const DirectX::XMMATRIX & matrix) { v = DirectX::XMQuaternionRotationMatrix(matrix); }
        inline explicit Quaternion(const float3 &v1From, const float3 &v2To) { Set(v1From, v2To); }
        inline explicit Quaternion(const DirectX::XMVECTOR & vec) { v = vec; }
        inline explicit Quaternion(const float4 q) { v = q; }
        inline explicit Quaternion(const DirectX::XMFLOAT4& q) { Set(q); }
        inline explicit Quaternion(const DirectX::XMVECTORF32& q) { v = q.v; }
        inline Quaternion(const Quaternion & in) noexcept { Set(in); } // copy
        inline Quaternion(Quaternion&& in) noexcept { v = std::move(in); } // move
        virtual ~Quaternion() = default;
        // Operators 
        inline Quaternion& operator= (const Quaternion & in) = default; // copy assignment
        inline Quaternion& operator= (Quaternion && in) = default; // move assignment
        inline Quaternion& operator= (const DirectX::FXMVECTOR& in) { v = in; return *this; }
        inline Quaternion& operator= (const DirectX::XMFLOAT4& in) { Set(in); return *this; }
        inline Quaternion operator~ (void) const { return Quaternion(DirectX::XMQuaternionConjugate(v)); }
        inline Quaternion operator- (void) const { return Quaternion(DirectX::XMVectorNegate(v)); }
        inline Quaternion operator- (const Quaternion rhs) const { return *this * rhs.Inverse(); }
        inline Quaternion operator+ (const Quaternion rhs) const { return Quaternion(DirectX::XMQuaternionMultiply(v, rhs)); }
        inline Quaternion operator* (const Quaternion rhs) const { return Quaternion(DirectX::XMQuaternionMultiply(v, rhs)); }
        inline Quaternion operator* (const float& scalerAngle) const { return Quaternion(GetAxis(), scalerAngle * GetAngleQuaternion()); }
        inline Quaternion operator/ (const Quaternion rhs) const { return *this * rhs.Inverse(); }
        inline float2 operator* (const float2 rhs) const { return float2(DirectX::XMVector3Rotate(float3(rhs, 0.f), v)); } // truncates z
        inline float3 operator* (const float3 rhs) const { return float3(DirectX::XMVector3Rotate(rhs, v)); }
        inline float3 operator* (const DirectX::XMVECTOR rhs) const { return float3(DirectX::XMVector3Rotate(rhs, v)); }
        inline Quaternion& operator*= (const Quaternion rhs) { *this = *this * rhs; return *this; } // same as adding two rotations (multiply the transforms)
        inline Quaternion& operator/= (const Quaternion rhs) { *this = *this * rhs.Inverse(); return *this; } // same as subtracting two rotations
        inline Quaternion& operator+= (const Quaternion rhs) { *this = *this * rhs; return *this; }
        inline Quaternion& operator-= (const Quaternion rhs) { *this = *this * rhs.Inverse(); return *this; }
        // Conversions
        inline operator bool() { return abs(GetW()) < 0.999998f ? true : false; } // if W is 1.0, there is no rotation to apply
        inline bool operator !() { return abs(GetW()) >= 0.999998f ? true : false; } // if W is 1.0, there is no rotation to apply
        operator char() = delete;
        operator int() = delete;
        operator float() = delete;
        inline operator DirectX::XMMATRIX() const { return DirectX::XMMatrixRotationQuaternion(v); }
        // Comparators
        inline bool operator== (const Quaternion& rhs) const noexcept { return DirectX::XMVector4Equal(v, rhs.GetVecConst()); }
        inline bool operator!= (const Quaternion& rhs) const noexcept { return DirectX::XMVector4NotEqual(v, rhs.GetVecConst()); }
        // Functionality
        inline virtual void MakeNormalize() { v = DirectX::XMQuaternionNormalize(v); }
        inline Quaternion   Inverse() const { return Quaternion(DirectX::XMQuaternionInverse(v)); }
        DirectX::XMFLOAT3   GetEulerAngles() const;
        DirectX::XMFLOAT3   CalculateAngularVelocity(const Quaternion previousRotation, float deltaTime) const;
        void                Validate() noexcept { if (DirectX::XMQuaternionIsNaN(v)) v = DirectX::XMQuaternionIdentity(); }
        // Accessors
        inline float3       GetAxis() const { float3 xyz = v; xyz.MakeNormalize(); return xyz; } // since v.xyz = N * sin(angle / 2), we can just re-normalized to retrieve the axis
        inline float        GetAngleEuler() const { auto a = std::atan2(DirectX::XMVectorGetX(DirectX::XMVector3Length(v)), DirectX::XMVectorGetW(v)); return a; } // [-π , +π] radians; euler angle about the axis
        inline float        GetAngleQuaternion() const { auto a = 2.0f * DirectX::XMScalarACos(GetW()); return a; } // [0 , +π] radians; quternion angle about the axis
        [[deprecated("GetAngleQuaternion() or GetAngleEuler() instead")]]
        inline float        GetAngle() const { auto a = 2.0f * DirectX::XMScalarACos(GetW()); return a; } // [0 , +π] radians; quternion angle about the axis
        inline DirectX::XMMATRIX GetRotationMatrix() const { return DirectX::XMMatrixRotationQuaternion(v); }
                                                                                                          // Assignments
        void __vectorcall   SetAxisAngle(float3 vector, float angleRadians); // [0, +π] radians
        inline void __vectorcall SetAxis(const float3 vector) { SetAxisAngle(vector, GetAngleQuaternion()); }
        inline void         SetAngle(const float angleRadians) { SetAxisAngle(GetAxis(), angleRadians); } // [0 , +π] radians
        inline void         SetEulerAngles(const float3 & eulerAngles) { v = DirectX::XMQuaternionRotationRollPitchYawFromVector(eulerAngles); }
        inline void         Set(const Quaternion & qIn) noexcept { v = qIn; Validate(); }
        void                Set(const float3 &vFrom, const float3 &vTo);
        void                Set(const DirectX::XMMATRIX& rotationMatrix) { v = DirectX::XMQuaternionRotationMatrix(rotationMatrix); }
        void                Set(const DirectX::XMFLOAT4& qIn) { v = DirectX::XMLoadFloat4(&qIn); Validate(); }
    };

    /******************************************************************************
    *   Conversions
    ******************************************************************************/
    inline uint2::uint2(const int2& in) { auto temp = in.Get_XMUINT2(); u[0] = temp.x; u[1] = temp.y; }
    inline uint2::uint2(const float2& in) { auto temp = in.Get_XMUINT2(); u[0] = temp.x; u[1] = temp.y; }
    inline int2::int2(const float2& in) { auto temp = in.Get_XMINT2(); i[0] = temp.x; i[1] = temp.y; }

    /******************************************************************************
    *   Math functions
    ******************************************************************************/
    inline float Random()
    {
        // Random number in range [-1,1]
        static bool once(true);
        if (once)
        {
            srand((UINT)time(NULL));
            once = false;
        }
        float r = (float)rand();
        r /= RAND_MAX;
        r = 2.0f * r - 1.0f;
        return r;
    }
    inline float Random(float min, float max)
    {
        static bool once(true);
        if (once)
        {
            srand((UINT)time(NULL));
            once = false;
        }
        float r = (float)rand();
        r /= RAND_MAX;
        r = (max - min) * r + min;
        return r;
    }
    inline float2 __vectorcall Random(const float2 min, const float2 max)
    {
        float2 res = float2(Random(min.GetX(), max.GetX()),
            Random(min.GetY(), max.GetY()));
        return res;
    }
    inline float3 __vectorcall Random(const float3 min, const float3 max)
    {
        float3 res = float3(Random(min.GetX(), max.GetX()),
            Random(min.GetY(), max.GetY()),
            Random(min.GetZ(), max.GetZ()));
        return res;
    }
    inline float Clamp(const float &v, const float &min, const float &max)
    {
        return v < min ? min : (v > max ? max : v);
    };
#define MAKE_SIMD_FUNCS( Type ) \
    inline Type __vectorcall Sqrt( Type s ) { return Type(XMVectorSqrt(s)); } \
    inline Type __vectorcall Recip( Type s ) { return Type(XMVectorReciprocal(s)); } \
    inline Type __vectorcall RecipSqrt( Type s ) { return Type(XMVectorReciprocalSqrtEst(s)); } \
    inline Type __vectorcall Floor( Type s ) { return Type(XMVectorFloor(s)); } \
    inline Type __vectorcall Ceiling( Type s ) { return Type(XMVectorCeiling(s)); } \
    inline Type __vectorcall Round( Type s ) { return Type(XMVectorRound(s)); } \
    inline Type __vectorcall Abs( Type s ) { return Type(XMVectorAbs(s)); } \
    inline Type __vectorcall Exp( Type s ) { return Type(XMVectorExp(s)); } \
    inline Type __vectorcall Pow( Type b, Type e ) { return Type(XMVectorPow(b, e)); } \
    inline Type __vectorcall Max( const Type a, const Type b ) { return Type(XMVectorMax(a, b)); } \
    inline Type __vectorcall Min( const Type a, const Type b ) { return Type(XMVectorMin(a, b)); } \
    inline Type __vectorcall Clamp( Type v, Type a, Type b ) { return XMVectorClamp(v, a, b); } \
    inline Type __vectorcall Lerp( Type a, Type b, Type t ) { return Type(XMVectorLerpV(a, b, t)); }

    MAKE_SIMD_FUNCS(float2);
    MAKE_SIMD_FUNCS(float3);
    MAKE_SIMD_FUNCS(float4);
#undef MAKE_SIMD_FUNCS
    inline float __vectorcall Dot(const float2 vec1In, const float2 vec2In) { return float2(DirectX::XMVector2Dot(vec1In, vec2In)).GetX(); } // order does not mater A•B = B•A
    inline float __vectorcall Dot(const float3 vec1In, const float3 vec2In) { return float3(DirectX::XMVector3Dot(vec1In, vec2In)).GetX(); }
    inline float __vectorcall Dot(const float4 vec1In, const float4 vec2In) { return float4(DirectX::XMVector4Dot(vec1In, vec2In)).GetX(); }

    inline float2 __vectorcall Cross(const float2 vec1In, const float2 vec2In) { return float2(DirectX::XMVector2Cross(vec1In, vec2In)); } // order does mater AxB = -(BxA)
    inline float3 __vectorcall Cross(const float3 vec1In, const float3 vec2In) { return float3(DirectX::XMVector3Cross(vec1In, vec2In)); } // note: this is LHS for DirectX, swap the terms for RHS
    inline float4 __vectorcall Cross(const float4 vec1In, const float4 vec2In, const float4 vec3In) { return float4(DirectX::XMVector4Cross(vec1In, vec2In, vec3In)); }
           
    inline float2 __vectorcall Normalize(const float2 vec1In) { return float2(DirectX::XMVector2Normalize(vec1In)); }
    inline float3 __vectorcall Normalize(const float3 vec1In) { return float3(DirectX::XMVector3Normalize(vec1In)); }
    inline float4 __vectorcall Normalize(const float4 vec1In) { return float4(DirectX::XMVector4Normalize(vec1In)); }
           
    inline uint2 Min(const uint2& a, const uint2& b) { return uint2((a.u[0] < b.u[0]) ? a.u[0] : b.u[0], (a.u[1] < b.u[1]) ? a.u[1] : b.u[1]); }
    inline uint2 Max(const uint2& a, const uint2& b) { return uint2((a.u[0] > b.u[0]) ? a.u[0] : b.u[0], (a.u[1] > b.u[1]) ? a.u[1] : b.u[1]); }
    inline uint2 Clamp(const uint2& c, const uint2& min, const uint2& max) { return King::Min(King::Max(c, min), max); }

    inline int2 Min(const int2& a, const int2& b) { return int2((a.i[0] < b.i[0]) ? a.i[0] : b.i[0], (a.i[1] < b.i[1]) ? a.i[1] : b.i[1]); }
    inline int2 Max(const int2& a, const int2& b) { return int2((a.i[0] > b.i[0]) ? a.i[0] : b.i[0], (a.i[1] > b.i[1]) ? a.i[1] : b.i[1]); }
    inline int2 Clamp(const int2& c, const int2& min, const int2& max) { return King::Min(King::Max(c, min), max); }

    inline int3 Min(const int3& a, const int3& b) { return int3((a.i[0] < b.i[0]) ? a.i[0] : b.i[0], (a.i[1] < b.i[1]) ? a.i[1] : b.i[1], (a.i[2] < b.i[2]) ? a.i[2] : b.i[2]); }
    inline int3 Max(const int3& a, const int3& b) { return int3((a.i[0] > b.i[0]) ? a.i[0] : b.i[0], (a.i[1] > b.i[1]) ? a.i[1] : b.i[1], (a.i[2] > b.i[2]) ? a.i[2] : b.i[2]); }
    inline int3 Clamp(const int3& c, const int3& min, const int3& max) { return King::Min(King::Max(c, min), max); }
    /******************************************************************************
    *   Streams
    ******************************************************************************/
    std::ostream& operator<< (std::ostream& os, const uint2& in);
    std::ostream& operator<< (std::ostream& os, const int2& in);
    std::ostream& operator<< (std::ostream& os, const int3& in);
    std::ostream& operator<< (std::ostream& os, const float2& in);
    std::ostream& operator<< (std::ostream& os, const float3& in);
    std::ostream& operator<< (std::ostream& os, const float4& in);
    std::ostream& operator<< (std::ostream& os, const DirectX::XMMATRIX& in);

    std::wostream& operator<< (std::wostream& os, const uint2& in);
    std::wostream& operator<< (std::wostream& os, const int2& in);
    std::wostream& operator<< (std::wostream& os, const int3& in);
    std::wostream& operator<< (std::wostream& os, const float2& in);
    std::wostream& operator<< (std::wostream& os, const float3& in);
    std::wostream& operator<< (std::wostream& os, const float4& in);

    std::istream& operator>> (std::istream& is, King::uint2& in);
    std::istream& operator>> (std::istream& is, King::int2& in);
    std::istream& operator>> (std::istream& is, King::float2& in);
    std::istream& operator>> (std::istream& is, King::float3& in);
    std::istream& operator>> (std::istream& is, King::float4& in);

    std::wistream& operator>> (std::wistream& is, King::uint2& in);
    std::wistream& operator>> (std::wistream& is, King::int2& in);
    std::wistream& operator>> (std::wistream& is, King::float2& in);
    std::wistream& operator>> (std::wistream& is, King::float3& in);
    std::wistream& operator>> (std::wistream& is, King::float4& in);

    /******************************************************************************
    *   json
    ******************************************************************************/
    void to_json(json& j, const uint2& from);
    void to_json(json& j, const int2& from);
    void to_json(json& j, const int3& from);
    void to_json(json& j, const float2& from);
    void to_json(json& j, const float3& from);
    void to_json(json& j, const float4& from);
    void to_json(json& j, const Quaternion& from);

    void from_json(const json& j, uint2& to);
    void from_json(const json& j, int2& to);
    void from_json(const json& j, int3& to);
    void from_json(const json& j, float2& to);
    void from_json(const json& j, float3& to);
    void from_json(const json& j, float4& to);
    void from_json(const json& j, Quaternion& to);


    /******************************************************************************
    *   SystemInfo
    *       Class for identifying the CPU and capabilities of the system
    *
    ******************************************************************************/
#ifndef DEVPKEY_Device_AdapterLuid
    // {60B193CB-5276-4D0F-96FC-F173ABAD3EC6} pid 3
    DEFINE_DEVPROPKEY(DEVPKEY_Device_AdapterLuid, 0x60b193cb, 0x5276, 0x4d0f, 0x96, 0xfc, 0xf1, 0x73, 0xab, 0xad, 0x3e, 0xc6, 3);
#endif
#ifndef GUID_DEVINTERFACE_DISPLAY_ADAPTER
    // {5B45201D-F2F2-4F3B-85BB-30FF1F953599}
    DEFINE_GUID(GUID_DEVINTERFACE_DISPLAY_ADAPTER,
        0x5b45201d, 0xf2f2, 0x4f3b, 0x85, 0xbb, 0x30, 0xff, 0x1f, 0x95, 0x35, 0x99);
#endif

    class SystemInfo
    {
    public:
        inline std::string GetSystemInfoString() const;

        inline std::string GetCPUInfoString() const;
        inline std::string GetCacheInfoString() const;
        inline std::string GetSIMDInfoString() const;

        inline std::string GetOSInfoString() const;
        inline std::string GetInstalledMemoryString() const;

        inline std::string GetGraphicsCardInfoString() const;
        inline std::string GetD3DInfoString() const;

        inline std::string GetGpuDriverVersionString() const;

    private:
        static inline std::string WideToUtf8(const wchar_t* ws);
        static inline bool TryGetPrimaryAdapterDesc(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter, DXGI_ADAPTER_DESC1& desc);
        static inline std::string FormatBytes(uint64_t bytes);
        static inline uint32_t CpuidLeaf0Eax();

        static inline bool CpuidHasLeaf(uint32_t leaf);
        static inline bool CpuidHasExtendedLeaf(uint32_t leaf);
        static inline uint32_t GetPhysicalCoreCount_Topology();
        static inline std::string FormatDriverVersion(const LARGE_INTEGER& v);

        static inline std::string GetGpuDriverVersion_SetupApi(const LUID& adapterLuid);
        static inline std::string GetFileVersionString(const std::wstring& path);
        static inline bool TryGetDeviceInstanceIdFromInterface(const wchar_t* deviceInterfacePath, std::wstring& instanceId);
    };

    inline std::string SystemInfo::GetSystemInfoString() const
    {
        std::ostringstream oss;
        oss << GetCPUInfoString();
        oss << GetCacheInfoString();
        oss << GetSIMDInfoString();
        oss << GetOSInfoString();
        oss << GetInstalledMemoryString();
        oss << GetGraphicsCardInfoString();
        oss << GetD3DInfoString();
        return oss.str();
    }

    inline uint32_t SystemInfo::CpuidLeaf0Eax()
    {
        int info[4] = {};
        __cpuid(info, 0);
        return static_cast<uint32_t>(info[0]);
    }

    inline std::string SystemInfo::WideToUtf8(const wchar_t* ws)
    {
        if (!ws || !*ws)
            return {};

        const int needed = ::WideCharToMultiByte(CP_UTF8, 0, ws, -1, nullptr, 0, nullptr, nullptr);
        if (needed <= 1)
            return {};

        std::string s;
        s.resize(static_cast<size_t>(needed - 1));
        ::WideCharToMultiByte(CP_UTF8, 0, ws, -1, s.data(), needed, nullptr, nullptr);
        return s;
    }

    inline std::string SystemInfo::FormatBytes(uint64_t bytes)
    {
        std::ostringstream oss;
        const double b = static_cast<double>(bytes);
        const double kb = 1024.0;
        const double mb = kb * 1024.0;
        const double gb = mb * 1024.0;

        oss << std::fixed << std::setprecision(2);
        if (b >= gb) oss << (b / gb) << " GB";
        else if (b >= mb) oss << (b / mb) << " MB";
        else if (b >= kb) oss << (b / kb) << " KB";
        else oss << bytes << " B";
        return oss.str();
    }

    inline std::string SystemInfo::GetCPUInfoString() const
    {
        // CPU brand string: CPUID 0x80000002..0x80000004 (if supported)
        int maxExt[4] = {};
        __cpuid(maxExt, 0x80000000);

        std::string brand;
        if (static_cast<unsigned int>(maxExt[0]) >= 0x80000004u)
        {
            char brandBuf[49] = {};
            int* p = reinterpret_cast<int*>(brandBuf);

            __cpuid(p + 0, 0x80000002);
            __cpuid(p + 4, 0x80000003);
            __cpuid(p + 8, 0x80000004);

            brand = brandBuf;
            // trim leading/trailing spaces
            while (!brand.empty() && brand.front() == ' ') brand.erase(brand.begin());
            while (!brand.empty() && brand.back() == ' ') brand.pop_back();
        }

        int info1[4] = {};
        __cpuid(info1, 1);

        std::ostringstream oss;
        oss << "" << (brand.empty() ? "Unknown" : brand) << "\n";
        return oss.str();
    }

    inline std::string SystemInfo::GetCacheInfoString() const
    {
        if (CpuidLeaf0Eax() < 4)
            return "Cache: Not supported\n";

        uint32_t l1 = 0, l2 = 0, l3 = 0;

        for (int i = 0; i < 16; ++i)
        {
            int r[4] = {};
            __cpuidex(r, 4, i);

            const uint32_t eax = static_cast<uint32_t>(r[0]);
            const uint32_t ebx = static_cast<uint32_t>(r[1]);
            const uint32_t ecx = static_cast<uint32_t>(r[2]);

            const uint32_t cacheType = eax & 0x1Fu;
            if (cacheType == 0)
                break;

            const uint32_t level = (eax >> 5) & 0x7u;

            const uint32_t lineSize = (ebx & 0xFFFu) + 1u;
            const uint32_t partitions = ((ebx >> 12) & 0x3FFu) + 1u;
            const uint32_t ways = ((ebx >> 22) & 0x3FFu) + 1u;
            const uint32_t sets = ecx + 1u;

            const uint64_t sizeBytes = static_cast<uint64_t>(lineSize) * partitions * ways * sets;
            const uint32_t sizeKB = static_cast<uint32_t>(sizeBytes / 1024u);

            if (level == 1) l1 += sizeKB;
            else if (level == 2) l2 += sizeKB;
            else if (level == 3) l3 += sizeKB;
        }

        std::ostringstream oss;
        oss << "";
        if (l1) oss << "L1 " << l1 << " KB ";
        if (l2) oss << "L2 " << l2 << " KB ";
        if (l3) oss << "L3 " << l3 << " KB ";
        if (!l1 && !l2 && !l3) oss << "Unknown";
        oss << "\n";
        return oss.str();
    }

    inline std::string SystemInfo::GetSIMDInfoString() const
    {
        int info[4] = {};
        __cpuid(info, 1);

        const unsigned int ecx = static_cast<unsigned int>(info[2]);
        const unsigned int edx = static_cast<unsigned int>(info[3]);

        const bool sse3 = (ecx & (1u << 0)) != 0;
        const bool ssse3 = (ecx & (1u << 9)) != 0;
        const bool sse41 = (ecx & (1u << 19)) != 0;
        const bool sse42 = (ecx & (1u << 20)) != 0;
        const bool fma = (ecx & (1u << 12)) != 0;

        const bool osxsave = (ecx & (1u << 27)) != 0;
        const bool avxCpu = (ecx & (1u << 28)) != 0;

        const bool sse = (edx & (1u << 25)) != 0;
        const bool sse2 = (edx & (1u << 26)) != 0;

        bool avxOs = false;
        if (osxsave)
        {
            const unsigned long long xcr0 = _xgetbv(0);
            avxOs = ((xcr0 & 0x6ull) == 0x6ull); // XMM (bit1) + YMM (bit2)
        }

        const bool avx = avxCpu && avxOs;

        bool avx2 = false;
        if (avx)
        {
            int info7[4] = {};
            __cpuidex(info7, 7, 0);
            const unsigned int ebx7 = static_cast<unsigned int>(info7[1]);
            avx2 = (ebx7 & (1u << 5)) != 0;
        }

        std::ostringstream oss;
        oss << ""
            << (sse ? "SSE " : "")
            << (sse2 ? "SSE2 " : "")
            << (sse3 ? "SSE3 " : "")
            << (ssse3 ? "SSSE3 " : "")
            << (sse41 ? "SSE4.1 " : "")
            << (sse42 ? "SSE4.2 " : "")
            << (fma ? "FMA " : "")
            << (avx ? "AVX " : "")
            << (avx2 ? "AVX2 " : "");

        if (!sse && !sse2 && !sse3 && !ssse3 && !sse41 && !sse42 && !fma && !avx && !avx2)
            oss << "None";

        oss << "\n";
        return oss.str();
    }

    inline std::string SystemInfo::GetOSInfoString() const
    {
        SYSTEM_INFO si = {};
        GetNativeSystemInfo(&si);

        BOOL wow64 = FALSE;
        using IsWow64Process2Fn = BOOL(WINAPI*)(HANDLE, USHORT*, USHORT*);
        auto isWow64Process2 = reinterpret_cast<IsWow64Process2Fn>(GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process2"));
        if (isWow64Process2)
        {
            USHORT processMachine = 0, nativeMachine = 0;
            if (isWow64Process2(GetCurrentProcess(), &processMachine, &nativeMachine))
                wow64 = (processMachine != 0);
        }
        else
        {
            IsWow64Process(GetCurrentProcess(), &wow64);
        }

        const bool os64 =
            (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ||
            (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64);

        const bool proc64 = os64 && !wow64;

        std::ostringstream oss;
        oss << "Windows ";
        oss << (os64 ? "64" : "32") << "-bit, System " << (proc64 ? "64" : "32") << "-bit\n";
        return oss.str();
    }

    inline std::string SystemInfo::GetInstalledMemoryString() const
    {
        MEMORYSTATUSEX memoryStatus = {};
        memoryStatus.dwLength = sizeof(memoryStatus);

        std::ostringstream oss;
        if (GlobalMemoryStatusEx(&memoryStatus))
            oss << "System Memory: " << FormatBytes(memoryStatus.ullTotalPhys) << "\n";
        else
            oss << "System Memory: Unknown\n";
        return oss.str();
    }

    inline bool SystemInfo::TryGetPrimaryAdapterDesc(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter, DXGI_ADAPTER_DESC1& desc)
    {
        adapter.Reset();
        ZeroMemory(&desc, sizeof(desc));

        Microsoft::WRL::ComPtr<IDXGIFactory6> factory;
        if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
            return false;

        // Prefer high-performance adapter if present.
        for (UINT i = 0;; ++i)
        {
            Microsoft::WRL::ComPtr<IDXGIAdapter1> a;
            if (FAILED(factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&a))))
                break;

            DXGI_ADAPTER_DESC1 d = {};
            if (FAILED(a->GetDesc1(&d)))
                continue;

            if (d.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            adapter = a;
            desc = d;
            return true;
        }

        // Fallback: first hardware adapter
        Microsoft::WRL::ComPtr<IDXGIFactory1> factory1;
        if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory1))))
            return false;

        for (UINT i = 0;; ++i)
        {
            Microsoft::WRL::ComPtr<IDXGIAdapter1> a;
            if (FAILED(factory1->EnumAdapters1(i, &a)))
                break;

            DXGI_ADAPTER_DESC1 d = {};
            if (FAILED(a->GetDesc1(&d)))
                continue;

            if (d.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            adapter = a;
            desc = d;
            return true;
        }

        return false;
    }

    inline std::string SystemInfo::GetGraphicsCardInfoString() const
    {
        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
        DXGI_ADAPTER_DESC1 desc = {};
        if (!TryGetPrimaryAdapterDesc(adapter, desc))
            return "GPU: Unknown (DXGI)\n";

        std::ostringstream oss;
        oss << "" << WideToUtf8(desc.Description) << "\n";
        oss << "VRAM: " << FormatBytes(desc.DedicatedVideoMemory)
            << ", SharedSystem " << FormatBytes(desc.SharedSystemMemory) << "\n";

        return oss.str();
    }

    inline std::string SystemInfo::GetD3DInfoString() const
    {
        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
        DXGI_ADAPTER_DESC1 desc = {};
        if (!TryGetPrimaryAdapterDesc(adapter, desc))
            return "D3D: Unknown\n";

        // Probe feature levels highest->lowest.
        static constexpr D3D_FEATURE_LEVEL kLevels[] =
        {
            D3D_FEATURE_LEVEL_12_2,
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        D3D_FEATURE_LEVEL chosen = static_cast<D3D_FEATURE_LEVEL>(0);

        for (auto fl : kLevels)
        {
            Microsoft::WRL::ComPtr<ID3D12Device> device;
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), fl, IID_PPV_ARGS(&device))))
            {
                chosen = fl;
                break;
            }
        }

        std::ostringstream oss;
        oss << "D3D12 Feature Level: ";
        switch (chosen)
        {
        case D3D_FEATURE_LEVEL_12_2: oss << "12_2"; break;
        case D3D_FEATURE_LEVEL_12_1: oss << "12_1"; break;
        case D3D_FEATURE_LEVEL_12_0: oss << "12_0"; break;
        case D3D_FEATURE_LEVEL_11_1: oss << "11_1"; break;
        case D3D_FEATURE_LEVEL_11_0: oss << "11_0"; break;
        case D3D_FEATURE_LEVEL_10_1: oss << "10_1"; break;
        case D3D_FEATURE_LEVEL_10_0: oss << "10_0"; break;
        default: oss << "Unknown"; break;
        }
        oss << "\n";
        return oss.str();
    }
    inline bool SystemInfo::CpuidHasLeaf(uint32_t leaf)
    {
        int info[4] = {};
        __cpuid(info, 0);
        return static_cast<uint32_t>(info[0]) >= leaf;
    }

    inline bool SystemInfo::CpuidHasExtendedLeaf(uint32_t leaf)
    {
        int info[4] = {};
        __cpuid(info, 0x80000000);
        return static_cast<uint32_t>(info[0]) >= leaf;
    }

    inline uint32_t SystemInfo::GetPhysicalCoreCount_Topology()
    {
        // Prefer 0x1F (V2 Extended Topology Enumeration), else 0x0B.
        const bool has1F = CpuidHasLeaf(0x1F);
        const bool has0B = CpuidHasLeaf(0x0B);

        const uint32_t leaf = has1F ? 0x1F : (has0B ? 0x0B : 0);
        if (leaf == 0)
            return 0;

        uint32_t smtShift = 0;
        uint32_t coreShift = 0;

        for (uint32_t level = 0; level < 32; ++level)
        {
            int r[4] = {};
            __cpuidex(r, static_cast<int>(leaf), static_cast<int>(level));

            const uint32_t eax = static_cast<uint32_t>(r[0]);
            const uint32_t ecx = static_cast<uint32_t>(r[2]);

            const uint32_t levelType = (ecx >> 8) & 0xFFu; // 1=SMT, 2=Core, 0=Invalid
            const uint32_t shift = eax & 0x1Fu;
            if (levelType == 0)
                break;

            if (levelType == 1) smtShift = shift;
            if (levelType == 2) { coreShift = shift; break; }
        }

        if (coreShift == 0)
            return 0;

        // With shifts known, enumerate all logical processors using current process affinity.
        // This avoids needing OS group enumeration and works for typical <=64 LP systems.
        DWORD_PTR processMask = 0, systemMask = 0;
        if (!GetProcessAffinityMask(GetCurrentProcess(), &processMask, &systemMask))
            return 0;

        struct PairHash { size_t operator()(uint64_t v) const noexcept { return static_cast<size_t>(v ^ (v >> 32)); } };

        std::unordered_set<uint64_t, PairHash> cores;
        for (uint32_t b = 0; b < sizeof(DWORD_PTR) * 8; ++b)
        {
            const DWORD_PTR bit = (static_cast<DWORD_PTR>(1) << b);
            if ((processMask & bit) == 0)
                continue;

            // Bind current thread to this logical processor.
            const HANDLE thread = GetCurrentThread();
            const DWORD_PTR prev = SetThreadAffinityMask(thread, bit);
            if (prev == 0)
                continue;

            int r1[4] = {};
            __cpuidex(r1, static_cast<int>(leaf), 0); // x2APIC id comes out in EDX for 0x0B/0x1F
            const uint32_t x2apicId = static_cast<uint32_t>(r1[3]);

            // Restore affinity.
            SetThreadAffinityMask(thread, prev);

            const uint32_t coreId = x2apicId >> smtShift;
            const uint32_t pkgId = x2apicId >> coreShift;

            const uint64_t key = (static_cast<uint64_t>(pkgId) << 32) | coreId;
            cores.insert(key);
        }

        return cores.empty() ? 0u : static_cast<uint32_t>(cores.size());
    }

    inline std::string SystemInfo::FormatDriverVersion(const LARGE_INTEGER& v)
    {
        // DXGI encodes as four 16-bit parts.
        const uint64_t u = static_cast<uint64_t>(v.QuadPart);
        const uint32_t a = static_cast<uint32_t>((u >> 48) & 0xFFFFu);
        const uint32_t b = static_cast<uint32_t>((u >> 32) & 0xFFFFu);
        const uint32_t c = static_cast<uint32_t>((u >> 16) & 0xFFFFu);
        const uint32_t d = static_cast<uint32_t>((u >> 0) & 0xFFFFu);

        std::ostringstream oss;
        oss << a << "." << b << "." << c << "." << d;
        return oss.str();
    }

    inline std::string SystemInfo::GetFileVersionString(const std::wstring& path)
    {
        DWORD handle = 0;
        const DWORD size = GetFileVersionInfoSizeW(path.c_str(), &handle);
        if (size == 0)
            return {};

        std::vector<BYTE> buf(size);
        if (!GetFileVersionInfoW(path.c_str(), 0, size, buf.data()))
            return {};

        VS_FIXEDFILEINFO* ffi = nullptr;
        UINT ffiLen = 0;
        if (!VerQueryValueW(buf.data(), L"\\", reinterpret_cast<void**>(&ffi), &ffiLen) || !ffi || ffiLen == 0)
            return {};

        const DWORD ms = ffi->dwFileVersionMS;
        const DWORD ls = ffi->dwFileVersionLS;

        std::ostringstream oss;
        oss << HIWORD(ms) << "." << LOWORD(ms) << "." << HIWORD(ls) << "." << LOWORD(ls);
        return oss.str();
    }

    inline bool SystemInfo::TryGetDeviceInstanceIdFromInterface(const wchar_t* deviceInterfacePath, std::wstring& instanceId)
    {
        instanceId.clear();

        CONFIGRET cr = CR_SUCCESS;
        DEVPROPTYPE type = 0;
        ULONG size = 0;

        cr = CM_Get_Device_Interface_PropertyW(
            deviceInterfacePath,
            &DEVPKEY_Device_InstanceId,
            &type,
            nullptr,
            &size,
            0);

        if (cr != CR_BUFFER_SMALL || size == 0)
            return false;

        std::vector<BYTE> buf(size);
        cr = CM_Get_Device_Interface_PropertyW(
            deviceInterfacePath,
            &DEVPKEY_Device_InstanceId,
            &type,
            buf.data(),
            &size,
            0);

        if (cr != CR_SUCCESS || type != DEVPROP_TYPE_STRING)
            return false;

        instanceId = reinterpret_cast<const wchar_t*>(buf.data());
        return !instanceId.empty();
    }

    inline std::string SystemInfo::GetGpuDriverVersion_SetupApi(const LUID& adapterLuid)
    {
        // Enumerate display adapters by interface: GUID_DEVINTERFACE_DISPLAY_ADAPTER
        HDEVINFO info = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_DISPLAY_ADAPTER, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
        if (info == INVALID_HANDLE_VALUE)
            return {};

        std::string result;

        for (DWORD index = 0;; ++index)
        {
            SP_DEVICE_INTERFACE_DATA ifData = {};
            ifData.cbSize = sizeof(ifData);

            if (!SetupDiEnumDeviceInterfaces(info, nullptr, &GUID_DEVINTERFACE_DISPLAY_ADAPTER, index, &ifData))
                break;

            DWORD required = 0;
            SetupDiGetDeviceInterfaceDetailW(info, &ifData, nullptr, 0, &required, nullptr);
            if (required == 0)
                continue;

            std::vector<BYTE> detailBuf(required);
            auto* detail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(detailBuf.data());
            detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);

            SP_DEVINFO_DATA devInfo = {};
            devInfo.cbSize = sizeof(devInfo);

            if (!SetupDiGetDeviceInterfaceDetailW(info, &ifData, detail, required, nullptr, &devInfo))
                continue;

            // Match this device to DXGI adapter via Adapter LUID (from DEVPROPKEY_Device_AdapterLuid)
            DEVPROPTYPE type = 0;
            BYTE luidBytes[sizeof(LUID)] = {};
            DWORD luidSize = sizeof(luidBytes);

            if (!SetupDiGetDevicePropertyW(info, &devInfo, &DEVPKEY_Device_AdapterLuid, &type, luidBytes, luidSize, &luidSize, 0))
                continue;

            if (type != DEVPROP_TYPE_UINT64 || luidSize != sizeof(LUID))
                continue;

            const LUID devLuid = *reinterpret_cast<const LUID*>(luidBytes);
            if (devLuid.HighPart != adapterLuid.HighPart || devLuid.LowPart != adapterLuid.LowPart)
                continue;

            // Get the driver store path (DEVPKEY_Device_DriverInfPath + resolve to actual binary via DriverDesc is messy).
            // A pragmatic approach: read the installed driver path from DEVPKEY_Device_DriverProvider / DriverVersion is not directly available,
            // so we query the "Driver" registry key and then read the "DriverVersion" from the main driver DLL (usually from DriverStore).
            // We'll use the device's "Driver" registry property to locate the driver key.
            wchar_t driverKey[512] = {};
            if (!SetupDiGetDeviceRegistryPropertyW(info, &devInfo, SPDRP_DRIVER, nullptr, reinterpret_cast<PBYTE>(driverKey), sizeof(driverKey), nullptr))
                continue;

            // Open: HKLM\System\CurrentControlSet\Control\Class\{4d36e968-e325-11ce-bfc1-08002be10318}\<driverKey>
            HKEY hClass = nullptr;
            if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}",
                0, KEY_READ, &hClass) != ERROR_SUCCESS)
            {
                continue;
            }

            HKEY hDriver = nullptr;
            const LONG rc = RegOpenKeyExW(hClass, driverKey, 0, KEY_READ, &hDriver);
            RegCloseKey(hClass);

            if (rc != ERROR_SUCCESS)
                continue;

            // Common value: "DriverVersion" sometimes exists, but not always. We'll prefer that if present.
            wchar_t verStr[128] = {};
            DWORD verType = 0;
            DWORD verCb = sizeof(verStr);
            if (RegQueryValueExW(hDriver, L"DriverVersion", nullptr, &verType, reinterpret_cast<LPBYTE>(verStr), &verCb) == ERROR_SUCCESS &&
                (verType == REG_SZ || verType == REG_EXPAND_SZ) && verStr[0] != 0)
            {
                result = WideToUtf8(verStr);
                RegCloseKey(hDriver);
                break;
            }

            // Fallback: try to get the user-mode driver DLL and read its file version.
            // Value names vary; "UserModeDriverName" is common on some stacks; try a few.
            const wchar_t* valueNames[] = { L"UserModeDriverName", L"OpenGLDriverName", L"UserModeDriverNameWoW" };

            std::wstring dllName;
            for (auto name : valueNames)
            {
                wchar_t tmp[260] = {};
                DWORD tmpType = 0;
                DWORD tmpCb = sizeof(tmp);
                if (RegQueryValueExW(hDriver, name, nullptr, &tmpType, reinterpret_cast<LPBYTE>(tmp), &tmpCb) == ERROR_SUCCESS &&
                    (tmpType == REG_SZ || tmpType == REG_EXPAND_SZ) && tmp[0] != 0)
                {
                    dllName = tmp;
                    break;
                }
            }

            RegCloseKey(hDriver);

            if (!dllName.empty())
            {
                // If only a filename is provided, assume System32.
                std::wstring path = dllName;
                if (path.find(L'\\') == std::wstring::npos)
                {
                    wchar_t sysDir[MAX_PATH] = {};
                    GetSystemDirectoryW(sysDir, MAX_PATH);
                    path = std::wstring(sysDir) + L"\\" + path;
                }

                const std::string fv = GetFileVersionString(path);
                if (!fv.empty())
                {
                    result = fv;
                    break;
                }
            }
        }

        SetupDiDestroyDeviceInfoList(info);
        return result;
    }
    inline std::string SystemInfo::GetGpuDriverVersionString() const
    {
        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
        DXGI_ADAPTER_DESC1 desc = {};
        if (!TryGetPrimaryAdapterDesc(adapter, desc))
            return "Driver: Unknown\n";

        std::string ver = GetGpuDriverVersion_SetupApi(desc.AdapterLuid);

        std::ostringstream oss;
        oss << "Driver: " << (ver.empty() ? "Unknown" : ver) << "\n";
        return oss.str();
    }
} // Kind namespace
