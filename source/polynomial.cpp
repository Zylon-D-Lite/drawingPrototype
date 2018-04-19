#include "./polynomial.hpp"
#include <algorithm>
void Polynomial::prettify() {
    //sort the expression based on exponent
    std::sort(expression.begin(), expression.end(),
    [](const Term& a, const Term& b)-> bool
    {
        return a.exponent > b.exponent;
    });
    int last_index = 0;
    for (int i = 1; i < expression.size(); ++i) {
        if (expression[i - 1].exponent != expression[i].exponent)
        {
            last_index = i;
        }
        else {
            expression[last_index].coefficient += expression[i].coefficient;
        }
    }
    auto end_it = std::unique(expression.begin(), expression.end(),
        [](const Term& a , const Term& b) ->bool
        {
            return a.exponent == b.exponent;
        });
    expression.erase(end_it, expression.end());
}

Polynomial operator*(const Polynomial& left, double right) {
    Polynomial ret = left;
    for (int i = 0; i < left.expression.size(); ++i) {
        ret[i].coefficient = left.expression[i].coefficient *right;
    }
    return ret;
}
Polynomial operator*(double left, const Polynomial &right) {
    return (right * left);
}

Polynomial operator*(const Polynomial& left, const Polynomial& right) {
    Polynomial ret;
    for (int i = 0; i < left.expression.size(); ++i) {
        for (int j = 0; j < right.expression.size(); ++j) {
            ret.push_back({left[i].coefficient * right[j].coefficient,
                     left[i].exponent + right[j].exponent});
        }
    }
    return ret;
}

Polynomial operator+(const Polynomial& left, const Polynomial & right) {
    Polynomial ret = left;
    ret.append(right.begin(), right.end());
    return ret;
}

Polynomial operator-(const Polynomial& p) {
    Polynomial ret = p;
    for (auto & i : ret.expression) {
        i.coefficient *= -1;
    }
    return ret;
}

Polynomial operator-(const Polynomial& left, const Polynomial& right) {
    Polynomial ret = -right;
    ret.append(left.begin(), left.end());
    return ret;
}