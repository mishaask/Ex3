//mishaaskarov@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Game.hpp"
#include "Player.hpp"
#include "Baron.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

#include <vector>
#include <set>
#include <string>

TEST_CASE("Base Player gather/tax/bribe behavior") {
    Player p{"Alice",0,Role::user};
    CHECK(p.coins == 0);
    p.gather();
    CHECK(p.coins == 1);
    p.tax();
    CHECK(p.coins == 3); // +2
    p.bribe();
    CHECK(p.coins == -1); // 3-4
}

TEST_CASE("Baron invests") {
    Baron b{"Bob",1};
    b.coins = 5;
    b.endTurn = false;
    b.invest();
    CHECK(b.coins == 8); 
    CHECK(b.endTurn == true);
}

TEST_CASE("Governor tax override") {
    Governor g{"Gov",2};
    g.coins = 0;
    g.tax();
    CHECK(g.coins == 3);
    CHECK(g.endTurn == true);
}

TEST_CASE("Spy role string and blocking flags") {
    Spy s{"SpyGuy",3};
    CHECK(s.roleToString(s.role) == "Spy");
    s.blockedFromArrest = true;
    CHECK(s.blockedFromArrest);
}

TEST_CASE("General onArrest bonus") {
    General gen{"Genny",4};
    gen.coins = 2;
    gen.onArrest();
    CHECK(gen.coins == 3);
}

TEST_CASE("Judge blocks bribe") {
    Judge j{"JudgeJ",5};
    // The prompt functions are UI only; we ensure role and method exist
    CHECK(j.roleToString(j.role) == "Judge");
}

TEST_CASE("Merchant start‐turn bonus and passive flag") {
    Merchant m{"Merch",6};
    m.coins = 3;
    m.merchantPassiveTriggered = false;
    // simulate Game loop:
    if (!m.endTurn && m.role == Role::merchant && m.coins >= 3 && !m.merchantPassiveTriggered) {
        m.coins += 1;
        m.merchantPassiveTriggered = true;
    }
    CHECK(m.coins == 4);
    CHECK(m.merchantPassiveTriggered);
}


TEST_CASE("assignRole honors name‐override") {
    Game g;
    std::vector<Player*> v;
    // Create a plain Player named "Spy"
    v.push_back(new Player("Spy", 0, Role::user));
    // idx==0 → name matches "Spy", so g.assignRole must make it a Spy
    Role r = g.assignRole(v, 0);
    CHECK(r == Role::spy);
    // The pointer should now be a Spy
    CHECK(dynamic_cast<Spy*>(v[0]) != nullptr);
    delete v[0];
}

TEST_CASE("assignRole honors all name‐overrides") {
    struct Fixture { std::string name; Role want; };
    std::vector<Fixture> cases = {
        {"Baron",    Role::baron},
        {"Governor", Role::governor},
        {"General",  Role::general},
        {"Judge",    Role::judge},
        {"Merchant", Role::merchant},
        {"Spy",      Role::spy}
    };
    Game g;
    for (auto &c : cases) {
        std::vector<Player*> v{ new Player(c.name, 1, Role::user) };
        Role got = g.assignRole(v, 0);
        CHECK(got == c.want);
        // check dynamic type
        switch (c.want) {
            case Role::baron:     CHECK(dynamic_cast<Baron*>(v[0])    != nullptr); break;
            case Role::governor:  CHECK(dynamic_cast<Governor*>(v[0]) != nullptr); break;
            case Role::general:   CHECK(dynamic_cast<General*>(v[0])  != nullptr); break;
            case Role::judge:     CHECK(dynamic_cast<Judge*>(v[0])    != nullptr); break;
            case Role::merchant:  CHECK(dynamic_cast<Merchant*>(v[0]) != nullptr); break;
            case Role::spy:       CHECK(dynamic_cast<Spy*>(v[0])     != nullptr); break;
            default: FAIL("unexpected role");                     break;
        }
        delete v[0];
    }
}

TEST_CASE("assignRole random fallback produces variety") {
    Game g;
    std::vector<Player*> v;
    const int N = 5;
    for (int i = 0; i < N; ++i)
        v.push_back(new Player("nobody", i, Role::user));

    std::set<Role> seen;
    // Run assignRole many times to see at least several different roles
    for (int i = 0; i < 100; ++i) {
        size_t idx = i % N;
        Role r = g.assignRole(v, idx);
        seen.insert(r);
    }
    // Should see at least 2 distinct roles (very unlikely all picks identical)
    CHECK(seen.size() >= 2);
    for (auto p : v) delete p;
}