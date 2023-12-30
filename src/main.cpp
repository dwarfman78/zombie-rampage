#include "../include/Application.hpp"
#include "../include/ApplicationServer.hpp"

int main(int argc, char **argv)
{

    std::string mode;
    if (argc > 1)
    {
        mode = argv[1];
    }
    else
    {
        mode = "";
    }

    if (mode == "--server")
    {
        ApplicationServer applicationServer(argc, argv);

        applicationServer.start();
    }
    else
    {

        Application application(argc, argv);

        application.start();
    }
    return EXIT_SUCCESS;
}
