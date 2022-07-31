#pragma once

namespace gr
{
    class Listener
    {
    public:
        static void SetPosition(float x, float y, float z);
        static void SetPosition(float position[3]);

        static void SetVelocity(float x, float y, float z);
        static void SetVelocity(float velocity[3]);

        static void SetOrientation(float x, float y, float z);
        static void SetOrientation(float orientation[3]);
    };
}