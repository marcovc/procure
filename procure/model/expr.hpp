/*
 * expr.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#ifndef PROCURE_EXPR_HPP_
#define PROCURE_EXPR_HPP_

namespace Procure {

template<class Eval>
struct IExpr
{
	virtual	Eval toLiteral() const = 0;
	virtual CP::DomExpr<Eval> toCPDomExpr(CP::Store& store) const = 0;
	virtual CP::BndExpr<Eval> toCPBndExpr(CP::Store& store) const = 0;
	virtual CP::ValExpr<Eval> toCPValExpr(CP::Store& store) const = 0;
	virtual Ref<Eval>	toRef(State& state) const = 0;
	virtual Expr<Eval>	bindRefs(State& state) const = 0;
	virtual Ref<CP::Var<Eval> >	toCPVarRef(State& state) const = 0;
	virtual State* getPState() const = 0;
	virtual std::ostream& print(std::ostream& os) const = 0;
	virtual std::string getTypeStr() const = 0;
	virtual ~IExpr() {}
};

} // PROCURE

#endif /* PROCURE_EXPR_HPP_ */
