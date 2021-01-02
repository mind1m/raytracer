#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record local_hit_rec;
    auto closest_t = t_max;
    bool hit_any = false;
    for (auto hobj : objects) {
        bool was_hit = hobj->hit(r, t_min, t_max, local_hit_rec);
        if (was_hit & (local_hit_rec.t < closest_t)) {
            rec = local_hit_rec;
            closest_t = local_hit_rec.t;
            hit_any = true;
        }
    }

    return hit_any;
}


#endif