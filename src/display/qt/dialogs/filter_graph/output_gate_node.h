/*
 * 2019 Tarpeeksi Hyvae Soft
 * 
 * Software: VCS
 * 
 */

#ifndef VCS_DISPLAY_QT_DIALOGS_FILTER_GRAPH_OUTPUT_GATE_NODE_H
#define VCS_DISPLAY_QT_DIALOGS_FILTER_GRAPH_OUTPUT_GATE_NODE_H

#include "filter/filter.h"
#include "display/qt/dialogs/filter_graph/base_filter_graph_node.h"

class OutputGateNode : public BaseFilterGraphNode
{
public:
    OutputGateNode(const QString title,
                   const unsigned width = 200,
                   const unsigned height = 130) : BaseFilterGraphNode(filter_node_type_e::gate, title, width, height)
    {
        this->edges =
        {
            node_edge_s(node_edge_s::direction_e::in, QRect(-8, 6, 24, 28), this),
        };

        return;
    }

    node_edge_s* input_edge(void) override;

private:
};

#endif
