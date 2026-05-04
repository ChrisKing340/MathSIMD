#include "MathSIMD.h"

using namespace King;
using namespace std;

/******************************************************************************
*   Streams
******************************************************************************/
std::ostream& King::operator<< (std::ostream& os, const King::uint2& in) { return os << "{ " << "x: " << in.GetX() << " y: " << in.GetY() << " }"; }
std::ostream& King::operator<< (std::ostream& os, const King::int2& in) { return os << "{ " << "x: " << in.GetX() << " y: " << in.GetY() << " }"; }
std::ostream& King::operator<< (std::ostream& os, const King::int3& in) { return os << "{ " << "x: " << in.GetX() << " y: " << in.GetY() << " z: " << in.GetZ() << " }"; }
std::ostream& King::operator<< (std::ostream& os, const King::float2& in) { return os << "{ " << "x: " << setw(9) << std::setprecision( 6 ) << in.f[0] << " y: " << setw(9) << in.f[1] << " }"; }
std::ostream& King::operator<< (std::ostream& os, const King::float3& in) { return os << "{ " << "x: " << setw(9) << std::setprecision( 6 ) << in.f[0] << " y: " << setw(9) << in.f[1] << " z: " << setw(9) << in.f[2] << " }"; }
std::ostream& King::operator<< (std::ostream& os, const King::float4& in) { return os << "{ " << "x: " << setw(9) << std::setprecision( 6 ) << in.f[0] << " y: " << setw(9) << in.f[1] << " z: " << setw(9) << in.f[2] << " w: " << setw(9) << in.f[3] << " }"; }

std::ostream& King::operator<<(std::ostream& os, const DirectX::XMMATRIX& in)
{
    os << "{{ " << "x1: " << setw(9) << float4(in.r[0]).f[0] << " y1: " << setw(9) << float4(in.r[0]).f[1] << " z1: " << setw(9) << float4(in.r[0]).f[2] << " w1: " << setw(9) << float4(in.r[0]).f[3] << " }\n";
    os << " { " << "x2: " << setw(9) << float4(in.r[1]).f[0] << " y2: " << setw(9) << float4(in.r[1]).f[1] << " z2: " << setw(9) << float4(in.r[1]).f[2] << " w2: " << setw(9) << float4(in.r[1]).f[3] << " }\n";
    os << " { " << "x3: " << setw(9) << float4(in.r[2]).f[0] << " y3: " << setw(9) << float4(in.r[2]).f[1] << " z3: " << setw(9) << float4(in.r[2]).f[2] << " w3: " << setw(9) << float4(in.r[2]).f[3] << " }\n";
    os << " { " << "x4: " << setw(9) << float4(in.r[3]).f[0] << " y4: " << setw(9) << float4(in.r[3]).f[1] << " z4: " << setw(9) << float4(in.r[3]).f[2] << " w4: " << setw(9) << float4(in.r[3]).f[3] << " }}\n";
    return os;
}

std::wostream& King::operator<< (std::wostream& os, const King::uint2& in) { return os << L"{ " << L"x: " << in.GetX() << L" y: " << in.GetY() << L" }"; }
std::wostream& King::operator<< (std::wostream& os, const King::int2& in) { return os << L"{ " << L"x: " << in.GetX() << L" y: " << in.GetY() << L" }"; }
std::wostream& King::operator<< (std::wostream& os, const King::int3& in) { return os << L"{ " << L"x: " << in.GetX() << L" y: " << in.GetY() << L" z: " << in.GetZ() << L" }"; }
std::wostream& King::operator<< (std::wostream& os, const King::float2& in) { return os << L"{ " << L"x: " << setw(9) << in.f[0] << L" y: " << setw(9) << in.f[1] << L" }"; }
std::wostream& King::operator<< (std::wostream& os, const King::float3& in) { return os << L"{ " << L"x: " << setw(9) << in.f[0] << L" y: " << setw(9) << in.f[1] << L" z: " << setw(9) << in.f[2] << L" }"; }
std::wostream& King::operator<< (std::wostream& os, const King::float4& in) { return os << L"{ " << L"x: " << setw(9) << in.f[0] << L" y: " << setw(9) << in.f[1] << L" z: " << setw(9) << in.f[2] << L" w: " << setw(9) << in.f[3] << L" }"; }

std::istream& King::operator>> (std::istream& is, King::uint2& in) { unsigned int ui[2]; is >> ui[0] >> ui[1]; in.Set(ui[0], ui[1]); return is; }
std::istream& King::operator>> (std::istream& is, int2& in) { int i[2]; is >> i[0] >> i[1]; in.Set(i[0], i[1]); return is; }
std::istream& King::operator>> (std::istream& is, float2& in) { DirectX::XMFLOAT2 f; is >> f.x >> f.y; in.Set(f); return is; }
std::istream& King::operator>> (std::istream& is, float3& in) { DirectX::XMFLOAT3 f; is >> f.x >> f.y >> f.z; in.Set(f); return is; }
std::istream& King::operator>> (std::istream& is, float4& in) { DirectX::XMFLOAT4 f; is >> f.x >> f.y >> f.z >> f.w; in.Set(f); return is; }

std::wistream& King::operator>> (std::wistream& is, King::uint2& in) { unsigned int ui[2]; is >> ui[0] >> ui[1]; in.Set(ui[0], ui[1]); return is; }
std::wistream& King::operator>> (std::wistream& is, int2& in) { int i[2]; is >> i[0] >> i[1]; in.Set(i[0], i[1]); return is; }
std::wistream& King::operator>> (std::wistream& is, float2& in) { DirectX::XMFLOAT2 f; is >> f.x >> f.y; in.Set(f); return is; }
std::wistream& King::operator>> (std::wistream& is, float3& in) { DirectX::XMFLOAT3 f; is >> f.x >> f.y >> f.z; in.Set(f); return is; }
std::wistream& King::operator>> (std::wistream& is, float4& in) { DirectX::XMFLOAT4 f; is >> f.x >> f.y >> f.z >> f.w; in.Set(f); return is; }

/******************************************************************************
*   json
******************************************************************************/
void King::to_json(json& j, const uint2& from) { j = json{ {"x", from.u[0]}, {"y", from.u[1]} }; }
void King::to_json(json& j, const int2& from) { j = json{ {"x", from.i[0]}, {"y", from.i[1]} }; }
void King::to_json(json& j, const int3& from) { j = json{ {"x", from.i[0]}, {"y", from.i[1]}, {"z", from.i[2]} }; }
void King::to_json(json& j, const float2& from) { j = json{ {"x", from.f[0]}, {"y", from.f[1]} }; }
void King::to_json(json& j, const float3& from) { j = json{ {"x", from.f[0]}, {"y", from.f[1]}, {"z", from.f[2]} }; }
void King::to_json(json& j, const float4& from) { j = json{ {"x", from.f[0]}, {"y", from.f[1]}, {"z", from.f[2]}, {"w", from.f[3]} }; }
void King::to_json(json& j, const Quaternion& from) { j = json{ {"x", from.f[0]}, {"y", from.f[1]}, {"z", from.f[2]}, {"w", from.f[3]} }; }

void King::from_json(const json& j, uint2& to) { j.at("x").get_to(to.u[0]); j.at("y").get_to(to.u[1]); }
void King::from_json(const json& j, int2& to) { j.at("x").get_to(to.i[0]); j.at("y").get_to(to.i[1]); }
void King::from_json(const json& j, int3& to) { j.at("x").get_to(to.i[0]); j.at("y").get_to(to.i[1]); j.at("z").get_to(to.i[2]); }
void King::from_json(const json& j, float2& to) { j.at("x").get_to(to.f[0]); j.at("y").get_to(to.f[1]); }
void King::from_json(const json& j, float3& to) { j.at("x").get_to(to.f[0]); j.at("y").get_to(to.f[1]); j.at("z").get_to(to.f[2]); }
void King::from_json(const json& j, float4& to) { j.at("x").get_to(to.f[0]); j.at("y").get_to(to.f[1]); j.at("z").get_to(to.f[2]); j.at("w").get_to(to.f[3]); }
void King::from_json(const json& j, Quaternion& to) { j.at("x").get_to(to.f[0]); j.at("y").get_to(to.f[1]); j.at("z").get_to(to.f[2]); j.at("w").get_to(to.f[3]); }

/******************************************************************************
*   Math functions and methods
******************************************************************************/
King::int3::int3(const float3& in)
{
    auto f = in.Get_XMFLOAT3();
    i[0] = static_cast<int>(f.x);
    i[1] = static_cast<int>(f.y);
    i[2] = static_cast<int>(f.z);
}

King::float2::float2(float3 vecIn)
{
    v = vecIn;
    DirectX::XMVectorSetZ(v, 0.f);
    DirectX::XMVectorSetW(v, 0.f);
}

King::float3::float3(float4 vecIn)
{
    v = vecIn;
    DirectX::XMVectorSetW(v, 0.f);
}

// Store the Euler angles in radians, credit to:
// http://www.gamedev.net/topic/597324-Quaternion-to-euler-angles-and-back-why-is-the-rotation-changing/
// returns [-π , +π] radians
DirectX::XMFLOAT3 King::Quaternion::GetEulerAngles() const
{
    DirectX::XMFLOAT3 pitchYawRoll; // output
    // x:ψ, y:θ, and z:φ

    float4 vn = DirectX::XMVector4Normalize(v);
    if (DirectX::XMVectorGetW(v) < 0.f)
        vn = -vn;

    float4 sqVec = DirectX::XMVectorMultiply(vn, vn);
    DirectX::XMFLOAT4 sq = sqVec;

    DirectX::XMFLOAT4 q;
    DirectX::XMStoreFloat4(&q, vn);

    // If Quaternion is normalized the unit is one, otherwise it is the correction factor
    //float unit = sqx + sqy + sqz + sqw; // = 1.0f if was normalized
    float unit = float4::SumComponents(sqVec);
    float test = q.x * q.y + q.z * q.w;

    if (test > 0.499998f * unit)
    {
        // Singularity at north pole
        pitchYawRoll.x = 0.f;
        pitchYawRoll.y = -2.f * atan2(q.x, q.w);
        pitchYawRoll.z = -DirectX::XM_PIDIV2;
        return pitchYawRoll;
    }
    else if (test < -0.499998f * unit)
    {
        // Singularity at south pole
        pitchYawRoll.x = 0.f;
        pitchYawRoll.y = 2.f * atan2(q.x, q.w);
        pitchYawRoll.z = DirectX::XM_PIDIV2;
        return pitchYawRoll;
    }
    // Wiki
    //pitchYawRoll.x = atan2(2.f * (q.x * q.w + q.y * q.z), (1.f - 2.f * (sq.x + sq.y)));
    //pitchYawRoll.y = asin(2.f * (q.y * q.w - q.x * q.z));
    //pitchYawRoll.z = atan2(2.f * (q.z * q.w + q.x * q.y), (1.f - 2.f * (sq.y + sq.z)));
  
    // MatLab
    //pitchYawRoll.x = atan2(2.f * (q.x * q.w + q.y * q.z), sq.w - sq.x - sq.y + sq.z);// range out is [-π, +π]
    //pitchYawRoll.y = asin(2.f * (q.y * q.w - q.x * q.z)); // [-π/2, +π/2]
    //pitchYawRoll.z = atan2(2.f * (q.z * q.w + q.x * q.y), sq.w + sq.x - sq.y - sq.z);// range out is [-π, +π]

    // (+)x = w/RHS, thumb down the (+) x axis for 180 deg (+PI), then(-PI) increasing back to zero for 360 deg.
    // (+)y = w/RHS, thumb down the (+) y axis for 180 deg (+PI), then(-PI) increasing back to zero for 360 deg.
    pitchYawRoll.x =  atan2(2.f * (q.x * q.w - q.y * q.z), -sq.x + sq.y - sq.z + sq.w); // range out is [-π, +π]
    pitchYawRoll.y =  atan2(2.f * (q.y * q.w - q.x * q.z), sq.x - sq.y - sq.z + sq.w); // range out is [-π, +π]
    pitchYawRoll.z =  asin(2.f * test / unit); // domain in is [-1.0, 1.0] and range out is [-π/2, + π/2]

    return pitchYawRoll;

}

DirectX::XMFLOAT3 King::Quaternion::CalculateAngularVelocity(const Quaternion prevRotation, float deltaTime) const
{
    // Calculate the angular velocity from a (delta rotation) / (unit time), credit to:
    // DavidSWu at https://forum.unity.com/threads/manually-calculate-angular-velocity-of-gameobject.289462/
    auto delta = *this - prevRotation;
    if (!delta) return delta; // zero

    float gain;
    float rotation = delta.GetW();
    // handle negative angles
    if (rotation < 0.0f)
    {
        const auto halfAngle = acos(-rotation);
        gain = -2.0f * halfAngle / (sin(halfAngle) * deltaTime);
    }
    else
    {
        const auto halfAngle = acos(rotation);
        gain = 2.0f * halfAngle / (sin(halfAngle) * deltaTime);
    }
    return float3(delta) * gain;
}

// Assignments

void __vectorcall King::Quaternion::SetAxisAngle(float3 vector, float angleRadians)
{
    // axis and angle is encoded into a float4, not stored directly eventhough
    // vector is float3 and angle is a float1.  The encoding allows for some nice
    // special algebra that makes the rotations easy to add and subtract.
    // angle = 2 * cosine function of w; if w = 0, angle = pi, w = 1, angle = 0

    if (angleRadians < 0.f)
    {
        angleRadians = abs(angleRadians);
        vector = -vector;
    }

    if (XMVector3Equal(vector, DirectX::XMVectorZero()))
    {
        v = vector;
        SetW(1.0f);
    }
    else
    {
        v = DirectX::XMQuaternionRotationAxis(vector, angleRadians);
        Validate();
    }
}

//https://stackoverflow.com/questions/1171849/finding-Quaternion-representing-the-rotation-from-one-vector-to-another
void King::Quaternion::Set(const float3 &vFrom, const float3 &vTo)
{
    float t = vFrom.DotProduct(vTo);

    if (t > 0.999999)
    {
        // parallel
        v = DirectX::XMQuaternionIdentity(); // 0 degrees
    }
    if (t < -0.999999)
    {
        // parallel and opposite
        v = float3::Normal(vFrom); // axis
        SetW(0.f); // 180 degree.
    }
    else
    {
        // axis
        v = float3::CrossProduct(vFrom, vTo);
        // angle
        SetW(1.f + t);

        v = DirectX::XMQuaternionNormalize(v);
    }

}
