#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <set>

#include "object_pool.hpp"

using object_pool::ObjectPool;

struct Particle {
    static int alive;  // 构造/析构计数，验证生命周期管理
    float x, y, vx, vy;
    Particle(float x, float y, float vx, float vy) : x(x), y(y), vx(vx), vy(vy) { ++alive; }
    ~Particle() { --alive; }
};
int Particle::alive = 0;

TEST_CASE("create 构造出正确的对象") {
    ObjectPool<Particle> pool;
    Particle* p = pool.create(1.f, 2.f, 3.f, 4.f);
    CHECK(p->x == 1.f);
    CHECK(p->vy == 4.f);
    CHECK(Particle::alive == 1);
    pool.destroy(p);
    CHECK(Particle::alive == 0);  // 析构确实被调用了
}

TEST_CASE("destroy 后槽位被复用（freelist LIFO）") {
    ObjectPool<Particle> pool;
    Particle* a = pool.create(0, 0, 0, 0);
    Particle* b = pool.create(0, 0, 0, 0);
    pool.destroy(a);
    pool.destroy(b);
    Particle* c = pool.create(1, 1, 1, 1);
    CHECK(c == b);  // 后进先出：最后释放的最先复用
    pool.destroy(c);
}

TEST_CASE("跨块增长：大量对象地址互不重叠且数值完好") {
    ObjectPool<Particle, 8> pool;  // 小块大小，强迫多次 grow
    std::set<Particle*> addrs;
    std::vector<Particle*> ps;
    for (int i = 0; i < 1000; ++i) {
        Particle* p = pool.create(i, i + 1, i + 2, i + 3);
        CHECK(addrs.insert(p).second);  // 地址唯一
        ps.push_back(p);
    }
    for (int i = 0; i < 1000; ++i) {
        CHECK(ps[i]->x == i);
        CHECK(ps[i]->vy == i + 3);
    }
    CHECK(Particle::alive == 1000);
    for (auto* p : ps) pool.destroy(p);
    CHECK(Particle::alive == 0);
}

TEST_CASE("释放一半再分配：不触发 grow 也能复用") {
    ObjectPool<Particle, 64> pool;
    std::vector<Particle*> ps;
    for (int i = 0; i < 64; ++i) ps.push_back(pool.create(i, 0, 0, 0));
    for (int i = 0; i < 32; ++i) pool.destroy(ps[i]);
    // 再创建 32 个，应当全部落在刚释放的槽位上（无需新块）
    for (int i = 0; i < 32; ++i) {
        Particle* p = pool.create(100 + i, 0, 0, 0);
        CHECK(p == ps[31 - i]);  // LIFO 倒序复用
    }
    // 64 个槽位现在全是活对象（前 32 个是重建的），统一销毁
    for (auto* p : ps) pool.destroy(p);
    CHECK(Particle::alive == 0);
}
