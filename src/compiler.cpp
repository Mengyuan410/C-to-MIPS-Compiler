#include "ast.hpp"

int main(int argc, char *argv[])
{
    Node *ast = parseAST(argv[2]);
    std::string indent = "  ";
    ContextPtr globalContext = new Context();
    ast->generateMIPS(indent,2,globalContext);
    return 0;
}
