#pragma once

#include "parser.hpp"
#include <cassert>
#include <map>
#include <unordered_map>

using namespace std;

class Generator{
public:
    inline explicit Generator(NodeProg prog)
        : m_prog(std::move(prog))
        {

    }

    void gen_term(const NodeTerm* term)
    {
        struct TermVisitor
        {
            Generator* gen;
            void operator()(const NodeTermIntLit* term_int_lit) const
            {
                gen->m_output << "    mov rax, " << term_int_lit->int_lit.value.value() << "\n";
                gen->push("rax");
            }
            void operator()(const NodeTermIdent* term_ident) const
            {
                if(!gen->m_vars.contains(term_ident->ident.value.value()))
                {
                    cerr << "ee variable declare cheydhittilla: " << term_ident->ident.value.value() << endl;
                    exit(EXIT_FAILURE);
                }
                else
                {
                    const auto& var = gen->m_vars.at(term_ident->ident.value.value());
                    stringstream offset;
                    offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1) * 8 << "]\n";
                    gen->push(offset.str());
                }

            }
        };
        TermVisitor visitor({ .gen = this});
        visit(visitor, term->var);
    }


    void gen_expr(const NodeExpr* expr)
    {
        struct ExprVisitor
        {
            Generator* gen;
            void operator()(const NodeTerm* term) const
            {
                gen->gen_term(term);
            }
            void operator()(const NodeBinExpr* bin_expr) const
            {
                gen->gen_expr(bin_expr->add->lhs);
                gen->gen_expr(bin_expr->add->rhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    add rax, rbx\n" ;
                gen->push("rax");
            }
        };

        ExprVisitor visitor{ .gen = this };
        visit(visitor,expr->var);
    }

    void gen_stmt(const NodeStmt* stmt)
    {
        struct StmtVisitor
        {
            Generator *gen;
            void operator()(const NodeStmtExit* stmt_exit) const
            {
                gen->gen_expr(stmt_exit->expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }
            void operator()(const NodeStmtLet* stmt_let)
            {
                if(gen->m_vars.contains(stmt_let->ident.value.value()))
                {
                    cerr << "ee Kanunna variable name already ond vere per kodkk" << stmt_let->ident.value.value() << endl;
                    exit(EXIT_FAILURE);
                }
                else
                {

                    gen->m_vars.insert({ stmt_let->ident.value.value(), Var { .stack_loc = gen->m_stack_size}});
                    gen->gen_expr(stmt_let->expr);
                }
            }
        };

        StmtVisitor visitor{ .gen = this };
        visit(visitor,stmt->var);
    }

    [[nodiscard]] string gen_prog()
    {

        m_output << "global _start\n_start:\n";

        for (const NodeStmt& stmt : m_prog.stmt)
        {
            gen_stmt(&stmt);
        }


        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0\n";
        m_output << "    syscall";
        return m_output.str();
    }
private:

    void push(const string& reg)
    {
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const string& reg)
    {
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    struct Var
    {
        size_t stack_loc;
    };

    const NodeProg m_prog;
    stringstream m_output;
    size_t m_stack_size = 0;
    unordered_map <string, Var> m_vars {};
};