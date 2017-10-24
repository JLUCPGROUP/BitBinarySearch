/*
 * BuildGecodeModel.h
 *
 *  Created on: 2016年9月17日
 *      Author: leezear
 */

#pragma once

#include "HModel.h"
#include "GModel.h"
using namespace Gecode;
enum PROP {
	PROP_BINARY,  ///< Use only binary disevars_uality constraints
	PROP_MIXED,   ///< Use single distinct and binary disevars_uality constraints
	PROP_DISTINCT ///< Use three distinct constraints
};
namespace cudacp {
static void BuildGModel(const HModel* h_model, GModel* g_model) {
	g_model->vars_ = IntVarArray(*g_model, h_model->vars.size());

	for (size_t i = 0; i < h_model->vars.size(); ++i) {
		HVar* v = h_model->vars[i];
		g_model->vars_[i] = IntVar(*g_model, IntSet(&*(v->vals.begin()), v->vals.size()));
	}

	for (size_t i = 0; i < h_model->tabs.size(); ++i) {
		HTab* tab = h_model->tabs[i];
		TupleSet ts;
		const int size = tab->tuples.size();
		const int arity = tab->scope.size();
		for (int j = 0; j < size; ++j)
			ts.add(IntArgs(arity, &*(tab->tuples[j].begin())));
		ts.finalize();

		IntVarArgs scope;
		for (int j = 0; j < arity; ++j)
			scope << g_model->vars_[tab->scope[j]->id];
		extensional(*g_model, scope, ts);
	}
};

static void BuildQueens(GModel* g_model, const int n, const PROP p = PROP_BINARY, const IntPropLevel ipl = IPL_DEF) {
	g_model->vars_ = IntVarArray(*g_model, n, 0, n - 1);

	switch (p) {
	case PROP_BINARY:
		for (int i = 0; i < n; i++)
			for (int j = i + 1; j < n; j++) {
				rel(*g_model, g_model->vars_[i] != g_model->vars_[j]);
				rel(*g_model, g_model->vars_[i] + i != g_model->vars_[j] + j);
				rel(*g_model, g_model->vars_[i] - i != g_model->vars_[j] - j);
			}
		break;
	case PROP_MIXED:
		for (int i = 0; i < n; i++)
			for (int j = i + 1; j < n; j++) {
				rel(*g_model, g_model->vars_[i] + i != g_model->vars_[j] + j);
				rel(*g_model, g_model->vars_[i] - i != g_model->vars_[j] - j);
			}
		distinct(*g_model, g_model->vars_, ipl);
		break;
	case PROP_DISTINCT:
		distinct(*g_model, IntArgs::create(n, 0, 1), g_model->vars_, ipl);
		distinct(*g_model, IntArgs::create(n, 0, -1), g_model->vars_, ipl);
		distinct(*g_model, g_model->vars_, ipl);
		break;
	}
};
}
