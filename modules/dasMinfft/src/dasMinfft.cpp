#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"

#include "dasMinfft.h"

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

    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasMinfft/src/dasMinfft.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_Minfft,Module_Minfft);
}

REGISTER_MODULE_IN_NAMESPACE(Module_Minfft, das);
