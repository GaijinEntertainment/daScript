 // require builtin




double div_by_zero_10447924851601705301 ( Context * __context__, double __num_rename_at_2 );
float div_by_zero_16131448245983416663 ( Context * __context__, float __num_rename_at_2 );
int32_t div_by_zero_6380333586457186393 ( Context * __context__, int32_t __num_rename_at_2 );
int64_t div_by_zero_16298140132315221801 ( Context * __context__, int64_t __num_rename_at_2 );
uint32_t div_by_zero_14519110232635966105 ( Context * __context__, uint32_t __num_rename_at_2 );
uint64_t div_by_zero_7511792572191008337 ( Context * __context__, uint64_t __num_rename_at_2 );
double mod_by_zero_1684415645314122274 ( Context * __context__, double __num_rename_at_12 );
float mod_by_zero_15587552727303661100 ( Context * __context__, float __num_rename_at_12 );
int32_t mod_by_zero_18078975840265032638 ( Context * __context__, int32_t __num_rename_at_12 );
int64_t mod_by_zero_13976890093854027822 ( Context * __context__, int64_t __num_rename_at_12 );
uint32_t mod_by_zero_7394646118292352510 ( Context * __context__, uint32_t __num_rename_at_12 );
uint64_t mod_by_zero_6240827892757477318 ( Context * __context__, uint64_t __num_rename_at_12 );
bool test_1207048600014507786 ( Context * __context__ );

void __init_script ( Context * __context__ )
{
}

double div_by_zero_10447924851601705301 ( Context * __context__, double __num_rename_at_2 )
{
	bool __ex_rename_at_3 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<double>::Div(__num_rename_at_2,0,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_3,true);
	});
	DAS_ASSERT((__ex_rename_at_3));
	return __num_rename_at_2;
}

float div_by_zero_16131448245983416663 ( Context * __context__, float __num_rename_at_2 )
{
	bool __ex_rename_at_3 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<float>::Div(__num_rename_at_2,0.f,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_3,true);
	});
	DAS_ASSERT((__ex_rename_at_3));
	return __num_rename_at_2;
}

int32_t div_by_zero_6380333586457186393 ( Context * __context__, int32_t __num_rename_at_2 )
{
	bool __ex_rename_at_3 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<int32_t>::Div(__num_rename_at_2,0,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_3,true);
	});
	DAS_ASSERT((__ex_rename_at_3));
	return __num_rename_at_2;
}

int64_t div_by_zero_16298140132315221801 ( Context * __context__, int64_t __num_rename_at_2 )
{
	bool __ex_rename_at_3 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<int64_t>::Div(__num_rename_at_2,0ll,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_3,true);
	});
	DAS_ASSERT((__ex_rename_at_3));
	return __num_rename_at_2;
}

uint32_t div_by_zero_14519110232635966105 ( Context * __context__, uint32_t __num_rename_at_2 )
{
	bool __ex_rename_at_3 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<uint32_t>::Div(__num_rename_at_2,0x0u,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_3,true);
	});
	DAS_ASSERT((__ex_rename_at_3));
	return __num_rename_at_2;
}

uint64_t div_by_zero_7511792572191008337 ( Context * __context__, uint64_t __num_rename_at_2 )
{
	bool __ex_rename_at_3 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<uint64_t>::Div(__num_rename_at_2,0x0ull,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_3,true);
	});
	DAS_ASSERT((__ex_rename_at_3));
	return __num_rename_at_2;
}

double mod_by_zero_1684415645314122274 ( Context * __context__, double __num_rename_at_12 )
{
	bool __ex_rename_at_13 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<double>::Mod(__num_rename_at_12,0,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_13,true);
	});
	DAS_ASSERT((__ex_rename_at_13));
	return __num_rename_at_12;
}

float mod_by_zero_15587552727303661100 ( Context * __context__, float __num_rename_at_12 )
{
	bool __ex_rename_at_13 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<float>::Mod(__num_rename_at_12,0.f,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_13,true);
	});
	DAS_ASSERT((__ex_rename_at_13));
	return __num_rename_at_12;
}

int32_t mod_by_zero_18078975840265032638 ( Context * __context__, int32_t __num_rename_at_12 )
{
	bool __ex_rename_at_13 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<int32_t>::Mod(__num_rename_at_12,0,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_13,true);
	});
	DAS_ASSERT((__ex_rename_at_13));
	return __num_rename_at_12;
}

int64_t mod_by_zero_13976890093854027822 ( Context * __context__, int64_t __num_rename_at_12 )
{
	bool __ex_rename_at_13 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<int64_t>::Mod(__num_rename_at_12,0ll,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_13,true);
	});
	DAS_ASSERT((__ex_rename_at_13));
	return __num_rename_at_12;
}

uint32_t mod_by_zero_7394646118292352510 ( Context * __context__, uint32_t __num_rename_at_12 )
{
	bool __ex_rename_at_13 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<uint32_t>::Mod(__num_rename_at_12,0x0u,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_13,true);
	});
	DAS_ASSERT((__ex_rename_at_13));
	return __num_rename_at_12;
}

uint64_t mod_by_zero_6240827892757477318 ( Context * __context__, uint64_t __num_rename_at_12 )
{
	bool __ex_rename_at_13 = false;
	das_try_recover(__context__, [&]()
	{
		return SimPolicy<uint64_t>::Mod(__num_rename_at_12,0x0ull,*__context__);
	}, [&]()
	{
		das_copy(__ex_rename_at_13,true);
	});
	DAS_ASSERT((__ex_rename_at_13));
	return __num_rename_at_12;
}

bool test_1207048600014507786 ( Context * __context__ )
{
	DAS_ASSERT((div_by_zero_6380333586457186393(__context__,1) == 1));
	DAS_ASSERT((mod_by_zero_18078975840265032638(__context__,1) == 1));
	DAS_ASSERT((div_by_zero_14519110232635966105(__context__,0x1u) == 0x1u));
	DAS_ASSERT((mod_by_zero_7394646118292352510(__context__,0x1u) == 0x1u));
	DAS_ASSERT((div_by_zero_16298140132315221801(__context__,1ll) == 1ll));
	DAS_ASSERT((mod_by_zero_13976890093854027822(__context__,1ll) == 1ll));
	DAS_ASSERT((div_by_zero_7511792572191008337(__context__,0x1ull) == 0x1ull));
	DAS_ASSERT((mod_by_zero_6240827892757477318(__context__,0x1ull) == 0x1ull));
	DAS_ASSERT((div_by_zero_16131448245983416663(__context__,1.f) == 1.f));
	DAS_ASSERT((mod_by_zero_15587552727303661100(__context__,1.f) == 1.f));
	DAS_ASSERT((div_by_zero_10447924851601705301(__context__,1) == 1));
	DAS_ASSERT((mod_by_zero_1684415645314122274(__context__,1) == 1));
	return true;
}
