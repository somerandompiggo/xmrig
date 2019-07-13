/* XMRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018-2019 SChernykh   <https://github.com/SChernykh>
 * Copyright 2016-2019 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XMRIG_CONFIG_H
#define XMRIG_CONFIG_H


#include <stdint.h>
#include <vector>


#include "backend/cpu/CpuConfig.h"
#include "base/kernel/config/BaseConfig.h"
#include "rapidjson/fwd.h"
#include "workers/CpuThreadLegacy.h"


namespace xmrig {


class IThread;


class Config : public BaseConfig
{
public:
    enum ThreadsMode {
        Automatic,
        Simple,
        Advanced
    };


    Config();

    bool read(const IJsonReader &reader, const char *fileName) override;
    void getJSON(rapidjson::Document &doc) const override;

    inline CnHash::AlgoVariant algoVariant() const       { return m_algoVariant; }
    inline bool isShouldSave() const                     { return (m_shouldSave || m_upgrade || m_cpu.isShouldSave()) && isAutoSave(); }
    inline const CpuConfig &cpu() const                  { return m_cpu; }
    inline const std::vector<IThread *> &threads() const { return m_threads.list; }
    inline int threadsCount() const                      { return static_cast<int>(m_threads.list.size()); }
    inline ThreadsMode threadsMode() const               { return m_threads.mode; }

private:
    bool finalize();
    void setAlgoVariant(int av);
    void setThreads(const rapidjson::Value &threads);

    CnHash::AlgoVariant getAlgoVariant() const;
#   ifdef XMRIG_ALGO_CN_LITE
    CnHash::AlgoVariant getAlgoVariantLite() const;
#   endif

    struct Threads
    {
       inline Threads() : mask(-1L), count(0), mode(Automatic) {}

       int64_t mask;
       size_t count;
       std::vector<CpuThreadLegacy::Data> cpu;
       std::vector<IThread *> list;
       ThreadsMode mode;
    };


    CnHash::AlgoVariant m_algoVariant;
    bool m_shouldSave;
    CpuConfig m_cpu;
    Threads m_threads;
};


} /* namespace xmrig */


#endif /* XMRIG_CONFIG_H */
