#include "Image.h"
#include "Filters.h"
#include "ArgumentParser.h"
#include "FilterController.h"
#include "help.h"
const Helper help;
int main(int argc, const char** argv) {
    if (argc == 1) {
        std::cout << help.readme;
        return 0;
    }
    Controller controller(argc, argv);
    Image image = controller.Read();
    controller.ApplyFilters(image);
    controller.Write(image);
}
