#pragma once

#include "eas_types.h"
#include "eas_sndlib.h"

// Usually it is not needed to directly call this function, DLSParser will invoke it
EAS_RESULT SF2Parser (EAS_HW_DATA_HANDLE hwInstData, EAS_FILE_HANDLE fileHandle, EAS_I32 offset, S_DLS **pDLS);
EAS_RESULT SF2Cleanup (EAS_HW_DATA_HANDLE hwInstData, S_DLS *pDLS);
