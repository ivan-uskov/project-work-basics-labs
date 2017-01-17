#pragma once
#include <atomic>

class LazyLoad
{
public:
    virtual ~LazyLoad() = default;

    void initialize()
    {
        if (isInitialized())
        {
            return;
        }

        doInitialize();
        initialized.store(true);
    }

    bool isInitialized() const
    {
        return initialized.load();
    }

protected:
    virtual void doInitialize() {}

private:
    std::atomic<bool> initialized = false;
};