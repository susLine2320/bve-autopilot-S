// パネル出力.cpp : 運転台パネルへ出力する値を定義します
//
// Copyright © 2019 Watanabe, Yuki
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110 - 1301  USA

#include "stdafx.h"
#include "パネル出力.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include "Main.h"
#include "共通状態.h"
#include "物理量.h"

namespace autopilot
{

    namespace
    {

        class tasc残距離桁
        {
        public:
            static constexpr int 無効値 = 11;

            constexpr tasc残距離桁(int 桁) noexcept : _桁{桁} { }
            ~tasc残距離桁() = default;

            int operator()(const Main &main) const;

        private:
            int _桁;
        };

        int tasc残距離桁::operator()(const Main &main) const
        {
            cm 残距離 =
                main.tasc状態().目標停止位置() - main.状態().現在位置();
            double 値 = 残距離.value;
            if (!std::isfinite(値)) {
                return 無効値;
            }
            int v = static_cast<int>(std::abs(値));
            for (int i = 0; i < _桁; ++i) {
                v /= 10;
            }
            if (v == 0) {
                return 10;
            }
            return v % 10;
        }

        const std::unordered_map<std::wstring, パネル出力対象> 対象名簿 = {
            {L"brake", パネル出力対象([](const Main & main) {
                return main.状態().前回制動指令().value;
            })},
            {L"power", パネル出力対象([](const Main & main) {
                return main.状態().前回力行ノッチ();
            })},
            {L"brakein", パネル出力対象([](const Main &main) {
                return main.状態().入力制動ノッチ().value;
            })},
            {L"powerin", パネル出力対象([](const Main &main) {
                return main.状態().入力力行ノッチ();
            })},

            {L"tascenabled", パネル出力対象([](const Main & main) {
                return main.tasc有効() && !main.ato有効();
            })},
            {L"tascmonitor", パネル出力対象([](const Main & main) {
                return main.tasc有効() && !main.ato有効() &&
                    main.tasc状態().制御中(main.状態().現在時刻());
            })},
            {L"tascbrake", パネル出力対象([](const Main & main) {
                if (!main.tasc有効() || main.ato一時停止中()) {
                    return 0;
                }
                return static_cast<int>(
                    main.tasc状態().出力ノッチ().制動成分().value);
            })},
                //add
            {L"tascbrake2", パネル出力対象([](const Main& main) {
                if (!main.tasc有効() || main.ato有効() || main.ato一時停止中() || main.状態().現在速度() == 0.0_mps || main.tasc状態().出力ノッチ().制動成分().value == 0) {
                    return 10;
                }
                return static_cast<int>(
                    (main.tasc状態().出力ノッチ().制動成分().value) / 10);
            })},
            {L"tascbrake1", パネル出力対象([](const Main& main) {
                if (!main.tasc有効() || main.ato有効() || main.ato一時停止中() || main.状態().現在速度() == 0.0_mps || main.tasc状態().出力ノッチ().制動成分().value == 0) {
                    return 10;
                }
                return static_cast<int>(
                    (main.tasc状態().出力ノッチ().制動成分().value) % 10);
            })},
            {L"tascstatusE", パネル出力対象([](const Main& main) {
                if (!main.tasc有効() || main.ato有効() || main.ato一時停止中() || main.状態().現在速度() == 0.0_mps || main.tasc状態().出力ノッチ().制動成分().value == 0) {
                    return 0;
                }
                else {
                    return 1;
                }
            })},
                //endadd
            {L"tascdistance", パネル出力対象([](const Main &main) {
                cm 残距離 =
                    main.tasc状態().目標停止位置() - main.状態().現在位置();
                double 値 = 残距離.value;
                if (!std::isfinite(値)) {
                    return 0;
                }
                return static_cast<int>(値);
            })},
            {L"tascdistancesign", パネル出力対象([](const Main &main) {
                auto 残距離 =
                    main.tasc状態().目標停止位置() - main.状態().現在位置();
                double 値 = 残距離.value;
                if (!std::isfinite(値)) {
                    return 0;
                }
                return 値 >= 0.0 ? 1 : 2;
            })},
            {L"tascdistancedm2", パネル出力対象(tasc残距離桁(0))},
            {L"tascdistancedm1", パネル出力対象(tasc残距離桁(1))},
            {L"tascdistanced0", パネル出力対象(tasc残距離桁(2))},
            {L"tascdistanced1", パネル出力対象(tasc残距離桁(3))},
            {L"tascdistanced2", パネル出力対象(tasc残距離桁(4))},
            {L"tascdistanced3", パネル出力対象(tasc残距離桁(5))},
            {L"tascdistanced4", パネル出力対象(tasc残距離桁(6))},
            {L"tascdistanced5", パネル出力対象(tasc残距離桁(7))},
            {L"tascposition", パネル出力対象([](const Main &main) {
                return main.tasc状態().定位置内(main.状態().現在位置());
            })},
            {L"inching", パネル出力対象([](const Main &main) {
                return main.インチング中();
            })},
            {L"atoenabled", パネル出力対象([](const Main & main) {
                return main.ato有効();
            })},
            {L"atostatus", パネル出力対象([](const Main &main) {
                if (!main.ato有効()) {
                    return 0;
                }
                if (main.ato状態().状態() != ato::制御状態::停止 &&
                    main.ato状態().状態() != ato::制御状態::一時停止)
                {
                    return 2;
                }
                if (!ato::発進可能(main.状態(), main.tasc状態())) {
                    return 1;
                }
                // 3 と 4 を交互に表示 (点滅)
                auto m =
                    std::fmod(main.状態().現在時刻().経過時間().value, 1.0);
                return m < 0.5 ? 3 : 4;
            })},
            {L"atopower", パネル出力対象([](const Main &main) {
                if (!main.ato有効()) {
                    return 0;
                }
                return static_cast<int>(
                    main.状態().前回自動出力().力行成分().value);
            })},
                //add
            { L"atostatusE", パネル出力対象([](const Main& main) {
                if (!main.ato有効()) {
                    return 0;
                }
                else if ((main.状態().前回自動出力().制動成分().value == 0 || main.状態().現在速度() == 0.0_mps) && main.状態().前回自動出力().力行成分().value == 0)
                {
                    return 0;
                }
                else if (main.状態().前回自動出力().力行成分().value >= 1) {
                    return 1;
                }
                else {
                    return 2;
                }
            })},
            {L"atopb2", パネル出力対象([](const Main &main) {
                if (!main.ato有効()) {
                    return 10;
                }
                else if ((main.状態().前回自動出力().制動成分().value == 0 || main.状態().現在速度() == 0.0_mps) && main.状態().前回自動出力().力行成分().value == 0)
                {
                    return 10;
                }
                else if (main.状態().前回自動出力().制動成分().value == 0) {
                    return static_cast<int>(
                        main.状態().前回自動出力().力行成分().value / 10);
                }
                return static_cast<int>(
                    main.状態().前回自動出力().制動成分().value / 10);
            })},
            { L"atopb1", パネル出力対象([](const Main& main) {
                if (!main.ato有効()) {
                    return 10;
                }
                else if ((main.状態().前回自動出力().制動成分().value == 0 || main.状態().現在速度() == 0.0_mps) && main.状態().前回自動出力().力行成分().value == 0)
                {
                    return 10;
                }
                else if (main.状態().前回自動出力().制動成分().value == 0) {
                    return static_cast<int>(
                        main.状態().前回自動出力().力行成分().value % 10);
                }
                return static_cast<int>(
                    main.状態().前回自動出力().制動成分().value % 10);
            })},
                //endadd
            { L"atobrake", パネル出力対象([](const Main& main) {
                if (!main.ato有効()) {
                    return 0;
                }
                return static_cast<int>(
                    main.状態().前回自動出力().制動成分().value);
            })},
            {L"powerthrottle", パネル出力対象([](const Main &main) {
                return main.ato有効() && main.力行抑止中();
            })},
            {L"speedlimit", パネル出力対象([](const Main & main) {
                kmph 制限速度 = main.現在制限速度();
                double 出力 = 制限速度.value;
                if (!std::isfinite(出力)) {
                    出力 = -20;
                }
                return static_cast<int>(std::round(出力));
            })},
            {L"speedpattern", パネル出力対象([](const Main & main) {
                kmph 制限速度 = main.現在常用パターン速度();
                double 出力 = 制限速度.value * 100;
                if (!std::isfinite(出力)) {
                    出力 = -20.0 * 100;
                }
                return static_cast<int>(std::round(出力));
            })},
            {L"orpspeedlimit", パネル出力対象([](const Main &main) {
                kmph 制限速度 = main.現在orp照査速度();
                double 出力 = 制限速度.value * 100;
                if (!std::isfinite(出力)) {
                    出力 = -20.0 * 100;
                }
                return static_cast<int>(std::round(出力));
            })},
            {L"compatmode", パネル出力対象([](const Main &main) {
                互換モード型 モード = main.状態().互換モード();
                return static_cast<int>(モード);
            })},
        };

        const パネル出力対象 無対象{ [](const Main &) { return 0; } };

    }

    パネル出力対象 パネル出力対象::対象(const std::wstring & 名前)
    {
        auto i = 対象名簿.find(名前);
        if (i == 対象名簿.end()) {
            return 無対象;
        }
        return i->second;
    }

}
