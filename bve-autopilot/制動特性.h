// 制動特性.h : 期待する減速度を得るために制動ノッチを加減します
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

#pragma once
#include <vector>
#include "加速度計.h"
#include "単位.h"

namespace autopilot
{

    /// このクラスは制動ノッチと減速度の関係を計算します。
    /// 実際に使用されている制動ノッチと実際の加速度を元に、
    /// 所望の加速度を得るためのノッチを推測します。
    class 制動特性
    {
    public:
        制動特性();
        ~制動特性();

        void 性能設定(
            int 常用ノッチ数, int 無効ノッチ数, int 拡張ノッチ数,
            加速度型 常用最大減速度, 時間型 反応時間,
            const std::vector<double> &pressure_rates);

        int 常用ノッチ数() const { return _常用ノッチ数; }
        int 無効ノッチ数() const { return _無効ノッチ数; }
        int 実効ノッチ数() const {
            return _常用ノッチ数 - _無効ノッチ数;
        }
        int 拡張ノッチ数() const { return _拡張ノッチ数; }
        int 自動ノッチ数() const {
            return _拡張ノッチ数 > 0 ? _拡張ノッチ数 : _常用ノッチ数;
        }
        加速度型 常用最大減速度() const { return _常用最大減速度; }
        時間型 反応時間() const { return _反応時間; }

        /// 所望の減速度を得るために設定すべき常用制動ノッチを得ます。
        /// 引数の大きさによっては常用最大を超えるノッチを返すことがあります。
        double ノッチ(加速度型 減速度) const;

        /// 常用制動ノッチに対して得られるであろう減速度を得ます。
        加速度型 減速度(double ノッチ) const;

    private:
        int _常用ノッチ数 = 0, _無効ノッチ数 = 0, _拡張ノッチ数 = 0;
        加速度型 _常用最大減速度 = 0;
        時間型 _反応時間 = 0;

        /// 車両パラメーターファイルの PressureRates に同じ。
        /// 少なくとも (_常用ノッチ数 + 1) 個の要素を持ちます。
        std::vector<double> _pressure_rates;

        /// _pressure_rates の要素数が足りないときに穴埋めします。
        void pressure_rates_を穴埋め();
    };

}
