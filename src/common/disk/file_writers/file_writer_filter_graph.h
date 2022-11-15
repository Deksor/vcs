/*
 * 2020 Tarpeeksi Hyvae Soft
 *
 * Software: VCS
 *
 */

#ifndef VCS_COMMON_DISK_FILE_WRITERS_FILE_WRITER_FILTER_GRAPH_H
#define VCS_COMMON_DISK_FILE_WRITERS_FILE_WRITER_FILTER_GRAPH_H

#include "display/display.h"

class BaseFilterGraphNode;

namespace file_writer
{
namespace filter_graph
{
    namespace version_b
    {
        bool write(const std::string &filename,
                   const std::vector<BaseFilterGraphNode*> &nodes,
                   const std::vector<filter_graph_option_s> &graphOptions);
    }
}
}

#endif
