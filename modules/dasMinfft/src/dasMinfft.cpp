#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"

#include "dasMinfft.h"

MAKE_TYPE_FACTORY(dct_plan, das::DctPlan)

namespace das {

inline float sqr(float x)
{
  return x * x;
}

static_assert(sizeof(minfft_real) == sizeof(float), "sizeof(minfft_real) != sizeof(float), define MINFFT_SINGLE");

void fft_real_forward(const TArray<float> & real_signal, TArray<float2> & complex_frequencies, Context * context, LineInfoArg * at)
{
  if (real_signal.size == 0 || ((real_signal.size + 1) & real_signal.size) == 0)
  {
    LOG(LogLevel::error) << "fft_real_forward: Length of input array must be power of 2 (length = " << int(real_signal.size) << ")";
    return;
  }

  minfft_aux * aux = minfft_mkaux_realdft_1d(real_signal.size);
  auto complexOutSize = real_signal.size / 2 + 1;
  if (complex_frequencies.size != complexOutSize)
    builtin_array_resize(complex_frequencies, complexOutSize, sizeof(float2), context, at);

  minfft_realdft((minfft_real *)real_signal.data, (minfft_cmpl *)complex_frequencies.data, aux);
  minfft_free_aux(aux);
}


void fft_calculate_magnitudes(const TArray<float2> & complex_frequencies, TArray<float> & magnitudes, Context * context, LineInfoArg * at)
{
  if (magnitudes.size != complex_frequencies.size)
    builtin_array_resize(magnitudes, complex_frequencies.size, sizeof(float), context, at);

  float2 * __restrict c = (float2 *)complex_frequencies.data;
  float * __restrict m = (float *)magnitudes.data;
  for (int i = 0, len = complex_frequencies.size; i < len; i++, m++, c++)
    *m = sqrtf(sqr(c->x) + sqr(c->y));
}

void fft_calculate_normalized_magnitudes(const TArray<float2> & complex_frequencies, TArray<float> & magnitudes, Context * context, LineInfoArg * at)
{
  if (magnitudes.size != complex_frequencies.size)
    builtin_array_resize(magnitudes, complex_frequencies.size, sizeof(float), context, at);

  if (!complex_frequencies.size)
    return;

  int len = complex_frequencies.size;
  float inv = 1.0f / len;

  float2 * __restrict c = (float2 *)complex_frequencies.data;
  float * __restrict m = (float *)magnitudes.data;
  for (int i = 0; i < len; i++, m++, c++)
    *m = sqrtf(sqr(c->x) + sqr(c->y)) * inv;
}

void fft_calculate_log_magnitudes(const TArray<float2> & complex_frequencies, TArray<float> & magnitudes, Context * context, LineInfoArg * at)
{
  if (magnitudes.size != complex_frequencies.size)
    builtin_array_resize(magnitudes, complex_frequencies.size, sizeof(float), context, at);

  if (!complex_frequencies.size)
    return;

  auto len = complex_frequencies.size;
  float offset = log2f(float(len));

  float2 * __restrict c = (float2 *)complex_frequencies.data;
  float * __restrict m = (float *)magnitudes.data;
  for (uint32_t i = 0; i < len; i++, m++, c++)
    *m = log2(sqr(c->x) + sqr(c->y) + 1e-10f) - offset;
}


void fft_real_inverse(const TArray<float2> & complex_frequencies, TArray<float> & real_signal, Context * context, LineInfoArg * at)
{
  auto p2 = complex_frequencies.size - 1;
  if (complex_frequencies.size <= 0 || ((p2 + 1) & p2) == 0)
  {
    LOG(LogLevel::error) << "fft_real_inverse: Length of input array must be (power of 2)+1, but length = " << int(complex_frequencies.size) << ")";
    return;
  }

  minfft_aux * aux = minfft_mkaux_realdft_1d(p2 * 2);
  auto realOutSize = p2 * 2;
  if (complex_frequencies.size != realOutSize)
    builtin_array_resize(real_signal, realOutSize, sizeof(float), context, at);

  minfft_invrealdft((minfft_cmpl *)complex_frequencies.data, (minfft_real *)real_signal.data, aux);
  minfft_free_aux(aux);
}


// ===== DCT (DCT-II forward / DCT-III inverse) =====

static inline bool is_pow2(int n)
{
  return n > 0 && (n & (n - 1)) == 0;
}

DctPlan::~DctPlan()
{
  if (aux)
    minfft_free_aux(aux);
}

// On any failure these return null (no handle to delete); callers can test the
// result with a null check.
DctPlan * make_dct_plan_1d(int n)
{
  if (!is_pow2(n))
  {
    LOG(LogLevel::error) << "make_dct_plan_1d: length must be a power of 2 (length = " << n << ")";
    return nullptr;
  }
  auto plan = new DctPlan();
  plan->aux = minfft_mkaux_t2t3_1d(n);
  if (!plan->aux)
  {
    LOG(LogLevel::error) << "make_dct_plan_1d: failed to allocate transform tables (length = " << n << ")";
    delete plan;
    return nullptr;
  }
  plan->n = n;
  return plan;
}

DctPlan * make_dct_plan_2d(int rows, int cols)
{
  if (!is_pow2(rows) || !is_pow2(cols))
  {
    LOG(LogLevel::error) << "make_dct_plan_2d: both dimensions must be powers of 2 (rows = " << rows << ", cols = " << cols << ")";
    return nullptr;
  }
  auto plan = new DctPlan();
  plan->aux = minfft_mkaux_t2t3_2d(rows, cols);
  if (!plan->aux)
  {
    LOG(LogLevel::error) << "make_dct_plan_2d: failed to allocate transform tables (rows = " << rows << ", cols = " << cols << ")";
    delete plan;
    return nullptr;
  }
  plan->n = rows * cols;
  return plan;
}

void dct(const TArray<float> & input, TArray<float> & output, DctPlan * plan, Context * context, LineInfoArg * at)
{
  if (!plan || !plan->aux)
  {
    LOG(LogLevel::error) << "dct: invalid plan (build it with make_dct_plan_1d / make_dct_plan_2d)";
    return;
  }
  if (int(input.size) != plan->n)
  {
    LOG(LogLevel::error) << "dct: input length " << int(input.size) << " does not match plan length " << plan->n;
    return;
  }
  if (int(output.size) != plan->n)
    builtin_array_resize(output, plan->n, sizeof(float), context, at);

  minfft_dct2((minfft_real *)input.data, (minfft_real *)output.data, plan->aux);
}

void idct(const TArray<float> & input, TArray<float> & output, DctPlan * plan, Context * context, LineInfoArg * at)
{
  if (!plan || !plan->aux)
  {
    LOG(LogLevel::error) << "idct: invalid plan (build it with make_dct_plan_1d / make_dct_plan_2d)";
    return;
  }
  if (int(input.size) != plan->n)
  {
    LOG(LogLevel::error) << "idct: input length " << int(input.size) << " does not match plan length " << plan->n;
    return;
  }
  if (int(output.size) != plan->n)
    builtin_array_resize(output, plan->n, sizeof(float), context, at);

  minfft_dct3((minfft_real *)input.data, (minfft_real *)output.data, plan->aux);
}


// canNew=false: plans must come from make_dct_plan_* (a bare `new dct_plan` would
// have aux==null and be unusable). canDelete=true keeps `delete plan`.
struct DctPlanAnnotation : ManagedStructureAnnotation<DctPlan,false,true> {
  DctPlanAnnotation(ModuleLibrary & ml)
    : ManagedStructureAnnotation("dct_plan", ml, "das::DctPlan") {
  }
};


class Module_Minfft : public Module {
public:
    Module_Minfft() : Module("minfft") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();

        addExtern<DAS_BIND_FUN(fft_real_forward)>(*this, lib,
          "fft_real_forward", SideEffects::modifyArgumentAndExternal, "fft_real_forward")
          ->args({"real_signal", "complex_frequencies", "", ""});

        addExtern<DAS_BIND_FUN(fft_real_inverse)>(*this, lib,
          "fft_real_inverse", SideEffects::modifyArgumentAndExternal, "fft_real_inverse")
          ->args({"complex_frequencies", "real_signal", "", ""});

        addExtern<DAS_BIND_FUN(fft_calculate_magnitudes)>(*this, lib,
          "fft_calculate_magnitudes", SideEffects::modifyArgumentAndExternal, "fft_calculate_magnitudes")
          ->args({"complex_frequencies", "magnitudes", "", ""});

        addExtern<DAS_BIND_FUN(fft_calculate_normalized_magnitudes)>(*this, lib,
          "fft_calculate_normalized_magnitudes", SideEffects::modifyArgumentAndExternal, "fft_calculate_normalized_magnitudes")
          ->args({"complex_frequencies", "normalized_magnitudes", "", ""});

        addExtern<DAS_BIND_FUN(fft_calculate_log_magnitudes)>(*this, lib,
          "fft_calculate_log_magnitudes", SideEffects::modifyArgumentAndExternal, "fft_calculate_log_magnitudes")
          ->args({"complex_frequencies", "log_magnitudes", "", ""});

        addAnnotation(new DctPlanAnnotation(lib));

        addExtern<DAS_BIND_FUN(make_dct_plan_1d)>(*this, lib,
          "make_dct_plan_1d", SideEffects::modifyExternal, "make_dct_plan_1d")
          ->args({"n"});

        addExtern<DAS_BIND_FUN(make_dct_plan_2d)>(*this, lib,
          "make_dct_plan_2d", SideEffects::modifyExternal, "make_dct_plan_2d")
          ->args({"rows", "cols"});

        addExtern<DAS_BIND_FUN(dct)>(*this, lib,
          "dct", SideEffects::modifyArgumentAndExternal, "dct")
          ->args({"input", "output", "plan", "", ""});

        addExtern<DAS_BIND_FUN(idct)>(*this, lib,
          "idct", SideEffects::modifyArgumentAndExternal, "idct")
          ->args({"input", "output", "plan", "", ""});

    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasMinfft/src/dasMinfft.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_Minfft,Module_Minfft);
}

REGISTER_MODULE_IN_NAMESPACE(Module_Minfft, das);
