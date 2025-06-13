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


struct NodeBinExprAdd{
    NodeExpr* lhs;
    NodeExpr* rhs;
};

//struct NodeBinExprMulti{
   // NodeExpr* lhs;
   // NodeExpr* rhs;
//}

struct NodeBinExpr{
    NodeBinExprAdd* add;
};

struct NodeTerm{
    variant<NodeTermIntLit* , NodeTermIdent*> var;
};

struct NodeExpr{
    variant<NodeTerm* ,NodeBinExpr* > var;
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
        else
        {
            return {};
        }
    }

    optional<NodeExpr*> parse_expr() {
        if (auto term = parse_term())
        {
            if (kazhich_nokk(TokenType::plus).has_value())
            {
                auto bin_expr = m_allocator.alloc<NodeBinExpr>();
                auto bin_expr_add = m_allocator.alloc<NodeBinExprAdd>();
                auto lhs_expr = m_allocator.alloc<NodeExpr>();
                lhs_expr->var = term.value();
                bin_expr_add->lhs = lhs_expr;
                if (auto rhs = parse_expr())
                {
                    bin_expr_add->rhs = rhs.value();
                    bin_expr->add = bin_expr_add;
                    auto expr = m_allocator.alloc<NodeExpr>();
                    expr->var = bin_expr;
                    return expr;
                }
                else
                {
                    cerr << "Add cheyyan olla adutha saanam koode tha enthonn kanikkana" << endl;
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                auto expr = m_allocator.alloc<NodeExpr>();
                expr->var = term.value();
                return expr;
            }
        }
        else
        {
            return {};
        }
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
