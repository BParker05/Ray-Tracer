#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rtweekend.h"

#include <vector>

class hittableList : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittableList() {}
    hittableList(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, double rayTmin, double rayTmax, hitRecord& rec) const override {
        hitRecord tempRec;
        bool hitAnything = false;
        auto closestSoFar = rayTmax;

        for (const auto& object : objects) {
            if (object->hit(r, rayTmin, closestSoFar, tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
};

#endif