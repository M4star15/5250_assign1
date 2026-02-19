// quad_order.h
// for order
#ifndef QUAD_ORDER_H
#define QUAD_ORDER_H

#include "quad.h"

int is_self_crossing(Quadrilateral q);
Quadrilateral sort_quad_ccw(Quadrilateral q);
Quadrilateral ensure_valid_order(Quadrilateral q);

#endif