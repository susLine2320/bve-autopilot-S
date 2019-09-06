// 共通状態.cpp : プラグイン全体で使用する、ゲーム全体の状態量です
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
#include <algorithm>
#include <cstdlib>
#include <limits>
#include "共通状態.h"
#include "単位.h"

#pragma warning(disable:4819)

namespace autopilot {

    void 共通状態::リセット()
    {
        // _設定.リセット(); // ファイルから読み込むのでリセットしない
        _互換モード = 互換モード型::無効;
        _状態 = ATS_VEHICLESTATE{};
        _加速度計.リセット();
        _勾配特性.消去();
    }

    void 共通状態::車両仕様設定(const ATS_VEHICLESPEC & 仕様)
    {
        _車両仕様 = 仕様;
        _制動特性.性能設定(
            仕様.BrakeNotches,
            std::max(std::min(仕様.AtsNotch, 仕様.BrakeNotches) - 1, 0),
            _設定.制動拡張ノッチ数(),
            _設定.常用最大減速度(),
            _設定.制動反応時間(),
            _設定.pressure_rates());
    }

    void 共通状態::地上子通過(const ATS_BEACONDATA & 地上子)
    {
        switch (地上子.Type)
        {
        case 1001: // 互換モード設定
            _互換モード = static_cast<互換モード型>(地上子.Optional);
            break;
        case 1008: // 勾配設定
            勾配追加(地上子.Optional);
            break;
        }
    }

    void 共通状態::経過(const ATS_VEHICLESTATE & 状態)
    {
        _状態 = 状態;
        _加速度計.経過({ mps_from_kmph(状態.Speed), 現在時刻() });
        _勾配特性.通過(現在位置() - 列車長());
    }

    void 共通状態::出力(const ATS_HANDLES & 出力)
    {
        _前回出力 = 出力;
    }

    void 共通状態::戸閉(bool 戸閉)
    {
        _戸閉 = 戸閉;
    }

    void 共通状態::逆転器操作(int ノッチ)
    {
        _逆転器ノッチ = ノッチ;
    }

    void 共通状態::力行操作(int ノッチ)
    {
        _力行ノッチ = ノッチ;
    }

    void 共通状態::制動操作(int ノッチ)
    {
        _制動ノッチ = ノッチ;
    }

    区間 共通状態::現在範囲() const
    {
        return 区間{ _状態.Location - 列車長(), _状態.Location };
    }

    加速度型 共通状態::進路勾配加速度(距離型 目標位置) const
    {
        区間 進路 = 現在範囲();
        進路.終点 = std::max(進路.終点, 目標位置);
        return _勾配特性.勾配加速度(進路);
    }

    加速度型 共通状態::車両勾配加速度() const
    {
        return _勾配特性.勾配加速度(現在範囲());
    }

    void 共通状態::勾配追加(int 地上子値)
    {
        if (地上子値 < -std::numeric_limits<int>::max()) {
            return; // std::abs でのオーバーフローを防止
        }

        bool 下り = 地上子値 < 0;
        地上子値 = std::abs(地上子値);

        double 距離 = 地上子値 / 1000;
        double 勾配 = (地上子値 % 1000) * 0.001;
        if (下り) {
            勾配 = -勾配;
        }
        _勾配特性.勾配区間追加(現在位置() + 距離, 勾配);
    }

}
