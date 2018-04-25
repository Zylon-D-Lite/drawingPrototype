#ifndef ZDL_POLYNOMIAL_H_
#define ZDL_POLYNOMIAL_H_
#include <utility>
#include <vector>
#include <cmath>
#include <string>

//! Term -- helper struct for Polynomial Class
struct Term {
    double coefficient;
    int exponent;
};

class Polynomial {
    friend Polynomial operator*(double, const Polynomial &);
    friend Polynomial operator*(const Polynomial&, double);
    friend Polynomial operator*(const Polynomial&, const Polynomial&);
    friend Polynomial operator+(const Polynomial&, const Polynomial&);
    friend Polynomial operator-(const Polynomial&, const Polynomial&);
    friend Polynomial operator-(const Polynomial&);
public:
    Polynomial() = default;
    Polynomial(const std::initializer_list<Term>& il) : Polynomial()
    {
        for (auto i : il) {
            expression.emplace_back(i);
        }
    }
    double calculate(double x) {
        double answer = 0;
        for (auto term : expression) {
            answer += term.coefficient * pow(x, term.exponent);
        }
        return answer;
    }

    void prettify();
    void push_back(const Term& t) { expression.push_back(t); }
    void pop_back() { expression.pop_back(); }
    void append(const std::vector<Term>::iterator& beg_it,
                const std::vector<Term>::iterator& end_it) {
        expression.insert(expression.end(), beg_it, end_it);
    }

    void append(const Polynomial& other) {
        expression.insert(expression.end(),
             other.expression.begin(), other.expression.end());
    }

    void append(__gnu_cxx::__normal_iterator
         <const Term*, std::vector<Term, std::allocator<Term>>> beg_it,
            __gnu_cxx::__normal_iterator
         <const Term*, std::vector<Term, std::allocator<Term>>> end_it) {
        expression.insert(expression.end(), beg_it, end_it);
     }
    std::vector<Term>::iterator begin() { return expression.begin(); }
    std::vector<Term>::iterator end() { return expression.end(); }

    auto begin() const { return expression.begin(); }
    auto end() const { return expression.end(); }
    std::vector<Term>::const_iterator cbegin() { return expression.cbegin(); }
    std::vector<Term>::const_iterator cend() { return expression.cend(); }

    Term& operator[](size_t index) { return expression[index]; }
    const Term& operator[](size_t index) const { return expression[index]; }


    std::string to_string() const {
        std::string ret;
        bool first_term = true;
        for (auto terms : expression) {
            if (terms.coefficient < 0.0000005 &&
                terms.coefficient > -0.0000005)
                    continue;
            std::string coefficient_str = std::to_string(std::abs(terms.coefficient));
            coefficient_str.erase(coefficient_str.find_last_not_of('0') + 1, std::string::npos);
            if (coefficient_str.back() == '.')
                coefficient_str += "0";
            if (first_term) {
                first_term = false;
                if (terms.coefficient < 0)
                    ret += "-";
                ret += coefficient_str;
                if (terms.exponent != 0)
                    ret += ("x^") + std::to_string(terms.exponent);
            }
            else {
                ret += (terms.coefficient < 0.0 ? " - " : " + ");
                ret += coefficient_str;
                if (terms.exponent != 0)
                    ret += ("x^") + std::to_string(terms.exponent);
            }
        }

        return ret;
    }


// Operators
private:
    std::vector<Term> expression;
};

Polynomial operator*(double, const Polynomial &);
Polynomial operator*(const Polynomial&, double);
Polynomial operator*(const Polynomial&, const Polynomial&);
Polynomial operator+(const Polynomial&, const Polynomial&);
Polynomial operator-(const Polynomial&, const Polynomial&);
Polynomial operator-(const Polynomial&);
#endif