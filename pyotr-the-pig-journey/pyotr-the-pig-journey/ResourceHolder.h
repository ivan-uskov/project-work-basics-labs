#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <mutex>

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    template <typename ...Parameters>
    void load(Identifier id, const std::string& filename, Parameters&&... params)
    {
        auto resource = std::make_unique<Resource>();
        if (!resource->loadFromFile(filename, std::forward<Parameters>(params)...))
        {
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
        }

        insertResource(std::move(id), std::move(resource));
    }

    Resource & get(Identifier id)
    {
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());

        return *found->second;
    }

    const Resource & get(Identifier id) const
    {
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());

        return *found->second;
    }

private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource)
    {
        mResourceMap.insert(std::make_pair(id, std::move(resource)));
    }

private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};
