#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    template <typename ...Parameters>
    Resource & load(Identifier id, const std::string& filename, Parameters&&... params)
    {
        auto resource = std::make_unique<Resource>();
        if (!resource->loadFromFile(filename, std::forward<Parameters>(params)...))
        {
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
        }

        insertResource(id, std::move(resource));

        return (*this)[id];
    }

    Resource & operator [] (Identifier id)
    {
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());

        return *found->second;
    }

    const Resource & operator [] (Identifier id) const
    {
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());

        return *found->second;
    }

private:
    void insertResource(const Identifier & id, std::unique_ptr<Resource> resource)
    {
        mResourceMap.insert(std::make_pair(id, std::move(resource)));
    }

private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};
