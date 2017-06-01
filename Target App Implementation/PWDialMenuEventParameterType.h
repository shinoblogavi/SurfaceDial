//!
//! @file  PWDialMenuEventParameterType.h
//! @brief ダイヤルメニューイベントパラメータタイプヘッダー
//!
//! @author rinaba
//!
#pragma once

#include "PWApplicationLib/PWProcessUniqueKeyType.h"

namespace Planeswalker
{

class	PWDialMenuEventParameter;

extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuSelected);		//!< ダイヤルメニューが選択された
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuClick);			//!< ダイヤルをクリックした
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuRotate);			//!< ダイヤルを回転した
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuContactBegin);	//!< ダイヤルを画面に接触した
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuContactContinue);	//!< ダイヤルを画面上で動かした
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuContactEnd);		//!< ダイヤルを画面から離した

}
