############################################################################
# QSkinny - Copyright (C) The authors
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

add_executable(tst_qskpopup tst_qskpopup.cpp)

target_link_libraries(tst_qskpopup PRIVATE qskinny qsktestsupport Qt${QT_VERSION_MAJOR}::Test)

set_target_properties(tst_qskpopup PROPERTIES FOLDER tests)

add_test(NAME tst_qskpopup COMMAND tst_qskpopup)
