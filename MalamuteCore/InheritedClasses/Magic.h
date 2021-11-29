// This class works by magic. At least, that's the best description of it I can think of. -dBob.

// Well actually it works by magic statics. But Magic is clearer. I think.

#ifndef MAGIC_H
#define MAGIC_H

template <typename T> class Magic final
{
public:
    static T& cast()
    {
        static T instance;
        return instance;
    }

private:
    Magic() = default;
    ~Magic() = default;

    Magic(const Magic&) = delete;
    Magic& operator=(const Magic&) = delete;
    Magic(Magic&&) = delete;
    Magic& operator= (Magic&&) = delete;
};

#endif // MAGIC_H
