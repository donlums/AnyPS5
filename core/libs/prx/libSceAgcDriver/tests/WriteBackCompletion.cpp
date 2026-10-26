#include "prx/libSceAgcDriver/Graphics/include/WriteBackCompletion.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

using AgcDriver::Graphics::CurrentWriteBackCompletion;

void Check(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "write-back completion: %s\n", message);
    std::exit(1);
}

int main() {
    std::array<std::uint64_t, 3> pending{17, 17, 0};
    Check(CurrentWriteBackCompletion(pending, 0, 17), "active first unit rejected");
    Check(CurrentWriteBackCompletion(pending, 1, 17), "active second unit rejected");
    Check(!CurrentWriteBackCompletion(pending, 2, 17), "cancelled unit accepted");
    Check(!CurrentWriteBackCompletion(pending, 3, 17), "outside unit accepted");
    Check(!CurrentWriteBackCompletion(pending, 0, 0), "empty completion ticket accepted");

    pending[0] = 18;
    Check(!CurrentWriteBackCompletion(pending, 0, 17), "older completion overwrote newer GPU work");
    Check(CurrentWriteBackCompletion(pending, 0, 18), "newer completion rejected");
    Check(CurrentWriteBackCompletion(pending, 1, 17), "independent pending unit lost");

    pending[1] = 0;
    Check(!CurrentWriteBackCompletion(pending, 1, 17), "cancelled completion resurrected CPU snapshot");
    pending[0] = 0;
    Check(!CurrentWriteBackCompletion(pending, 0, 18), "completed ticket was replayed");
    std::puts("write-back completion tickets respect replacement, cancellation and independent units");
}
