#pragma once

#define VERIFY_SUCCEEDED(x) \
    Assert::IsTrue(SUCCEEDED(x))
