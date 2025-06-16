#pragma once
#include <iostream>
#include <optional>
#include "arena.hpp"
#include "tokenization.hpp"
#include <variant>
using namespace std;

struct NodeTermIdent{
    Token ident;
};

struct NodeTermIntLit{
    Token int_lit;
};

struct NodeExpr;

struct NodeTermParen{
    NodeExpr* expr;
};

struct NodeBinExprAdd{
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExprSub{
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExprMulti{
   NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExprDiv{
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExpr{
    variant<NodeBinExprAdd*, NodeBinExprMulti*, NodeBinExprSub*, NodeBinExprDiv*> var;
};

struct NodeTerm{
    variant<NodeTermIntLit* , NodeTermIdent*, NodeTermParen*> var;
};

struct NodeExpr{
    variant<NodeTerm* ,NodeBinExpr*> var;
};



struct NodeStmtLet{
    NodeExpr* expr;
    Token ident;
};

struct NodeStmtExit{
    NodeExpr* expr;
};

struct NodeStmt{
    variant<NodeStmtLet* ,NodeStmtExit* > var;
};

struct NodeProg{
    vector<NodeStmt> stmt;
};

class Parser {
public:
    inline explicit Parser(const vector<Token>& tokens)
       : m_tokens(std::move(tokens)) , m_allocator(1024 * 1024 * 4) // 4 mb
    {
    }

    optional<NodeTerm*> parse_term()
    {

        if(auto int_lit = kazhich_nokk(TokenType::int_lit))
        {
            auto term_int_lit = m_allocator.alloc<NodeTermIntLit>();
            term_int_lit->int_lit = int_lit.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = term_int_lit;
            return term;
        }
        else if(auto ident = kazhich_nokk(TokenType::ident))
        {
            auto term_ident = m_allocator.alloc<NodeTermIdent>();
            term_ident->ident = ident.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = term_ident;
            return term;
        }
        else if(auto paren = kazhich_nokk(TokenType::open_paren))
        {
            auto expr = parse_expr();
            if(!expr.has_value())
            {
                cerr << "Expected Expr" << endl;
                exit(EXIT_FAILURE);
            }
            kazhich_nokk(TokenType::close_paren, "expected ')'");
            auto term_paren = m_allocator.alloc<NodeTermParen>();
            term_paren->expr = expr.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = term_paren;
            return term;
        }
        else
        {
            return {};
        }
    }

    optional<NodeExpr*> parse_expr(int min_prec = 0)
    {
        optional<NodeTerm*> term_lhs = parse_term();
        if(!term_lhs.has_value())
        {
            return{};
        }
        auto expr_lhs = m_allocator.alloc<NodeExpr>();
        expr_lhs->var = term_lhs.value();

        while(true)
        {
            optional<Token> cur_token = peek();
            optional<int> prec;
            if(!cur_token.has_value()) {
                break;
            }
                prec = bin_prec(cur_token->type);
                if (!prec.has_value()) {
                    break;

                }
                if(prec.value() < min_prec)
                {
                    break;
                }


            Token op = kazhikk();
            int next_min_prec = prec.value() + 1;
            auto expr_rhs = parse_expr(next_min_prec);
            if(!expr_rhs.has_value())
            {
                cerr << "Unable to parse expr" << endl;
                exit(EXIT_FAILURE);
            }

            auto expr = m_allocator.alloc<NodeBinExpr>();
            auto expr_lhs2 = m_allocator.alloc<NodeExpr>();
            if(op.type == TokenType::plus){
                auto add = m_allocator.alloc<NodeBinExprAdd>();
                expr_lhs2->var = expr_lhs->var;
                add->lhs = expr_lhs2;
                add->rhs = expr_rhs.value();
                expr->var = add;
            }
            else if(op.type == TokenType::star){
                auto mult = m_allocator.alloc<NodeBinExprMulti>();
                expr_lhs2->var = expr_lhs->var;
                mult->lhs = expr_lhs2;
                mult->rhs = expr_rhs.value();
                expr->var = mult;
            }
            else if(op.type == TokenType::sub){
                auto sub = m_allocator.alloc<NodeBinExprSub>();
                expr_lhs2->var = expr_lhs->var;
                sub->lhs = expr_lhs2;
                sub->rhs = expr_rhs.value();
                expr->var = sub;
            }
            else if(op.type == TokenType::div){
                auto div = m_allocator.alloc<NodeBinExprDiv>();
                expr_lhs2->var = expr_lhs->var;
                div->lhs = expr_lhs2;
                div->rhs = expr_rhs.value();
                expr->var = div;
            }
            expr_lhs->var = expr;

        }
        return expr_lhs;
    }

    optional<NodeStmt> parse_stmt()
    {
        if(peek().value().type == TokenType::exit && peek(1).value().type == TokenType::open_paren)
        {
            kazhikk(); // exit
            kazhikk(); // (
            auto stmt_exit = m_allocator.alloc<NodeStmtExit>();

            if(auto node_expr = parse_expr())
            {
                stmt_exit->expr = node_expr.value();
            }
            else
            {
                cerr << "valid Expression alla" << endl;
                exit(EXIT_FAILURE);
            }
            kazhich_nokk(TokenType::close_paren, "')' Aaru idum?");
            kazhich_nokk(TokenType::semi, "';' Aaru idum?");

            return NodeStmt { .var = stmt_exit };
        }
        else if(peek().has_value() && peek().value().type == TokenType::let &&
                peek(1).has_value() && peek(1).value().type == TokenType::ident &&
                peek(2).has_value() && peek(2).value().type == TokenType::eq)
        {
            kazhikk(); // let
            auto stmt_let = m_allocator.alloc<NodeStmtLet>();
            stmt_let->ident = kazhikk(); // ident
            kazhikk(); // =

            if(auto expr = parse_expr())
            {
                stmt_let->expr = expr.value();
            }
            else
            {
                cerr << "Valid expression kodukk myre" << endl;
                exit(EXIT_FAILURE);
            }
            kazhich_nokk(TokenType::semi, "';' Aaru idum?");
            return NodeStmt { .var = stmt_let };
        }
        else
        {
            return {};
        }
    }


    optional<NodeProg> parse_prog()
    {
        NodeProg prog;
        while (peek().has_value())
        {
            if(auto stmt = parse_stmt())
            {
                prog.stmt.push_back(stmt.value());
            }
            else
            {
                cerr << "Maryadekk olla statement tha" << endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }
private:
    const string m_src;
    ArenaAllocator m_allocator;
    size_t m_index = 0;
    const vector<Token> m_tokens;

    [[nodiscard]] inline optional<Token> peek(int offset = 0) const{
        if(m_index + offset >= m_tokens.size()) {
            return {};
        }
        else{
            return m_tokens.at(m_index + offset);
        }

    }

    inline Token kazhikk(){
        return m_tokens.at(m_index++);
    }

    inline Token kazhich_nokk(TokenType type, const string& err_msg)
    {
        if(peek().has_value() && peek().value().type == type )
        {
            return kazhikk();
        }
        else
        {
            cerr << err_msg << endl;
            exit(EXIT_FAILURE);
        }

    }
    inline optional<Token> kazhich_nokk(TokenType type)
    {
        if (peek().has_value() && peek().value().type == type)
        {
            return kazhikk();
        }
        else
        {
            return {};
        }
    }

};
