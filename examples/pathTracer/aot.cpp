#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

void saveTga ( const char * fileName, das::Array & arr, int width, int height, das::Context * context );

namespace das {

    template <typename TT>
    TT & cast_vec_ref ( vec4f & local ) {
        return *(TT *)(&local);
    }

    namespace aot {
        TypeInfo __type_info__f293c4e8 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 12, 0xf293c4e8 };
        TypeInfo __type_info__a7069c83 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 4, 0xa7069c83 };
        TypeInfo __type_info__fa15db70 = { Type::tFloat, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 12, 0xfa15db70 };
        TypeInfo * __tinfo_0[8] = { &__type_info__f293c4e8, &__type_info__a7069c83, &__type_info__fa15db70, &__type_info__a7069c83, &__type_info__fa15db70, &__type_info__a7069c83, &__type_info__f293c4e8, &__type_info__a7069c83 };

        TypeInfo * __tinfo_1[3] = { &__type_info__a7069c83, &__type_info__fa15db70, &__type_info__a7069c83 };

        enum class Type{
            Dielectric = 2,
            Lambert = 0,
            Metal = 1,
        };

        struct Camera {
            float3 origin;
            float3 lowerLeftCorner;
            float3 horizontal;
            float3 vertical;
            float3 u;
            float3 v;
            float3 w;
            float lensRadius;
        };
        static_assert(sizeof(Camera)==88,"structure size mismatch with DAS");

        struct Hit {
            float3 pos;
            float3 normal;
            float t;
        };
        static_assert(sizeof(Hit)==28,"structure size mismatch with DAS");

        struct Material {
            enum Type mtype;
            float3 albedo;
            float3 emissive;
            float roughness;
            float ri;
        };
        static_assert(sizeof(Material)==36,"structure size mismatch with DAS");

        struct Ray {
            float3 orig;
            float3 dir;
        };
        static_assert(sizeof(Ray)==24,"structure size mismatch with DAS");

        struct Sphere {
            float3 center;
            float radius;
            float radius2;
            float iRadius;
        };
        static_assert(sizeof(Sphere)==24,"structure size mismatch with DAS");

        struct Camera Camera ( Context * __context__, float3 lookFrom, float3 lookAt, float3 vup, float vfov, float aspect, float aperture, float focusDist );
        void draw ( Context * __context__, float time, int32_t frameCount, int32_t screenWidth, int32_t screenHeight, int32_t ymin, int32_t ymax, TArray<float3> & backbuffer, int32_t & outRayCount );
        struct Ray getRay ( Context * __context__, struct Camera & that, float s, float t );
        int32_t hitSpheres ( Context * __context__, const struct Ray & r, float tMin, float tMax, struct Hit & outHit );
        bool hitWorld ( Context * __context__, const struct Ray & r, float tMin, float tMax, struct Hit & outHit, int32_t & outID );
        int32_t job ( Context * __context__, TArray<float3> & backbuffer, int32_t frameCount, int32_t width, int32_t height, int32_t ymin, int32_t ymax );
        float3 pointAt ( Context * __context__, const struct Ray & ray, float t );
        void prepare ( Context * __context__ );
        void resize ( Context * __context__, TArray<float3> & Arr, int32_t newSize );
        bool scatter ( Context * __context__, const struct Material & mat, const struct Ray & r_in, struct Hit & rec, float3 & attenuation, struct Ray & scattered, float3 & outLightE, int32_t & inoutRayCount );
        float schlick ( Context * __context__, float cosine, float ri );
        bool test ( Context * __context__ );
        float3 trace ( Context * __context__, struct Ray & r, int32_t depth, int32_t & inoutRayCount, bool doMaterialE );
        int32_t trace ( Context * __context__, int32_t screenWidth, int32_t screenHeight, int32_t frameCount, int32_t ymin, int32_t ymax, TArray<float3> & backbuffer, struct Camera & cam );

        void __init_script ( Context * __context__ )
        {
            das_global_zero<int4,0>(__context__); /*RAND_SEED*/
            das_global<TDim<struct Material,9>,16>(__context__) = (([&]() -> TDim<struct Material,9> {
                TDim<struct Material,9> __mks_102; das_zero(__mks_102);
                __mks_102(0,__context__).mtype = Type::Lambert;
                __mks_102(0,__context__).albedo = float3(0.800000012f,0.800000012f,0.800000012f);
                __mks_102(0,__context__).emissive = float3(0.f,0.f,0.f);
                __mks_102(0,__context__).roughness = 0.f;
                __mks_102(0,__context__).ri = 0.f;
                __mks_102(1,__context__).mtype = Type::Lambert;
                __mks_102(1,__context__).albedo = float3(0.800000012f,0.400000006f,0.400000006f);
                __mks_102(1,__context__).emissive = float3(0.f,0.f,0.f);
                __mks_102(1,__context__).roughness = 0.f;
                __mks_102(1,__context__).ri = 0.f;
                __mks_102(2,__context__).mtype = Type::Lambert;
                __mks_102(2,__context__).albedo = float3(0.400000006f,0.800000012f,0.400000006f);
                __mks_102(2,__context__).emissive = float3(0.f,0.f,0.f);
                __mks_102(2,__context__).roughness = 0.f;
                __mks_102(2,__context__).ri = 0.f;
                __mks_102(3,__context__).mtype = Type::Metal;
                __mks_102(3,__context__).albedo = float3(0.400000006f,0.400000006f,0.800000012f);
                __mks_102(3,__context__).emissive = float3(0.f,0.f,0.f);
                __mks_102(3,__context__).roughness = 0.f;
                __mks_102(3,__context__).ri = 0.f;
                __mks_102(4,__context__).mtype = Type::Metal;
                __mks_102(4,__context__).albedo = float3(0.400000006f,0.800000012f,0.400000006f);
                __mks_102(4,__context__).emissive = float3(0.f,0.f,0.f);
                __mks_102(4,__context__).roughness = 0.f;
                __mks_102(4,__context__).ri = 0.f;
                __mks_102(5,__context__).mtype = Type::Metal;
                __mks_102(5,__context__).albedo = float3(0.400000006f,0.800000012f,0.400000006f);
                __mks_102(5,__context__).emissive = float3(0.f,0.f,0.f);
                __mks_102(5,__context__).roughness = 0.200000003f;
                __mks_102(5,__context__).ri = 0.f;
                __mks_102(6,__context__).mtype = Type::Metal;
                __mks_102(6,__context__).albedo = float3(0.400000006f,0.800000012f,0.400000006f);
                __mks_102(6,__context__).emissive = float3(0.f,0.f,0.f);
                __mks_102(6,__context__).roughness = 0.600000024f;
                __mks_102(6,__context__).ri = 0.f;
                __mks_102(7,__context__).mtype = Type::Dielectric;
                __mks_102(7,__context__).albedo = float3(0.400000006f,0.400000006f,0.400000006f);
                __mks_102(7,__context__).emissive = float3(0.f,0.f,0.f);
                __mks_102(7,__context__).roughness = 0.f;
                __mks_102(7,__context__).ri = 1.5f;
                __mks_102(8,__context__).mtype = Type::Lambert;
                __mks_102(8,__context__).albedo = float3(0.800000012f,0.600000024f,0.200000003f);
                __mks_102(8,__context__).emissive = float3(30.f,25.f,15.f);
                __mks_102(8,__context__).roughness = 0.f;
                __mks_102(8,__context__).ri = 0.f;
                return __mks_102;
            })()); /*s_SphereMats*/
            das_global<TDim<struct Sphere,9>,352>(__context__) = (([&]() -> TDim<struct Sphere,9> {
                TDim<struct Sphere,9> __mks_89; das_zero(__mks_89);
                __mks_89(0,__context__).center = float3(0.f,-100.5f,-1.f);
                __mks_89(0,__context__).radius = 100.f;
                __mks_89(1,__context__).center = float3(2.f,0.f,-1.f);
                __mks_89(1,__context__).radius = 0.5f;
                __mks_89(2,__context__).center = float3(0.f,0.f,-1.f);
                __mks_89(2,__context__).radius = 0.5f;
                __mks_89(3,__context__).center = float3(-2.f,0.f,-1.f);
                __mks_89(3,__context__).radius = 0.5f;
                __mks_89(4,__context__).center = float3(2.f,0.f,1.f);
                __mks_89(4,__context__).radius = 0.5f;
                __mks_89(5,__context__).center = float3(0.f,0.f,1.f);
                __mks_89(5,__context__).radius = 0.5f;
                __mks_89(6,__context__).center = float3(-2.f,0.f,1.f);
                __mks_89(6,__context__).radius = 0.5f;
                __mks_89(7,__context__).center = float3(0.5f,1.f,0.5f);
                __mks_89(7,__context__).radius = 0.5f;
                __mks_89(8,__context__).center = float3(-1.5f,1.5f,0.f);
                __mks_89(8,__context__).radius = 0.300000012f;
                return __mks_89;
            })()); /*s_Spheres*/
        }

        struct Camera Camera ( Context * __context__, float3 lookFrom, float3 lookAt, float3 vup, float vfov, float aspect, float aperture, float focusDist )
        {
            struct Camera that; das_zero(that);
            that.lensRadius = aperture / 2.f;
            float theta = ((vfov * 3.14159274f) / 180.f);
            float halfHeight = SimPolicy<float>::Tan(theta / 2.f,*__context__);
            float halfWidth = (aspect * halfHeight);
            that.origin = lookFrom;
            that.w = normalize4(SimPolicy<float3>::Sub(lookFrom,lookAt,*__context__));
            that.u = normalize4(cross3(vup,that.w));
            that.v = cross3(that.w,that.u);
            that.lowerLeftCorner = SimPolicy<float3>::Sub((SimPolicy<float3>::Sub((SimPolicy<float3>::Sub(that.origin,(SimPolicy<float3>::MulScalVec(cast<float>::from((halfWidth * focusDist)),that.u,*__context__)),*__context__)),(SimPolicy<float3>::MulScalVec(cast<float>::from((halfHeight * focusDist)),that.v,*__context__)),*__context__)),(SimPolicy<float3>::MulScalVec(cast<float>::from(focusDist),that.w,*__context__)),*__context__);
            that.horizontal = SimPolicy<float3>::MulScalVec(cast<float>::from(((2.f * halfWidth) * focusDist)),that.u,*__context__);
            that.vertical = SimPolicy<float3>::MulScalVec(cast<float>::from(((2.f * halfHeight) * focusDist)),that.v,*__context__);
            return that;
        }

        void draw ( Context * __context__, float time, int32_t frameCount, int32_t screenWidth, int32_t screenHeight, int32_t ymin, int32_t ymax, TArray<float3> &  backbuffer, int32_t & outRayCount )
        {
            int32_t rayCount = 0;
            struct Camera cam = Camera(__context__,float3(0.f,2.f,3.f),float3(0.f,0.f,0.f),float3(0.f,1.f,0.f),60.f,float(screenWidth) / float(screenHeight),0.100000001f,3.f);
            rayCount += trace(__context__,screenWidth,screenHeight,frameCount,ymin,ymax,das_arg<TArray<float3>>::pass(backbuffer),das_arg<struct Camera>::pass(cam));
            outRayCount = rayCount;
        }

        struct Ray getRay ( Context * __context__, struct Camera &  that, float s, float t )
        {
            vec4f /*float3*/ rd = (SimPolicy<float3>::MulScalVec(cast<float>::from(that.lensRadius),randomInUnitDisk(das_global<int4,0>(__context__) /*RAND_SEED*/),*__context__));
            vec4f /*float3*/ offset = SimPolicy<float3>::MadS(that.u,cast<float>::from(v_extract_x(rd) /*x*/),SimPolicy<float3>::MulVecScal(that.v,cast<float>::from(v_extract_y(rd) /*y*/),*__context__),*__context__);
            return (([&]() -> struct Ray {
                struct Ray __mks_73; das_zero(__mks_73);
                __mks_73.orig = (SimPolicy<float3>::Add(that.origin,offset,*__context__));
                __mks_73.dir = normalize4(SimPolicy<float3>::Sub((SimPolicy<float3>::Sub(SimPolicy<float3>::MadS(that.vertical,cast<float>::from(t),SimPolicy<float3>::MadS(that.horizontal,cast<float>::from(s),that.lowerLeftCorner,*__context__),*__context__),that.origin,*__context__)),offset,*__context__));
                return __mks_73;
            })());
        }

        int32_t hitSpheres ( Context * __context__, const struct Ray &  r, float tMin, float tMax, struct Hit &  outHit )
        {
            float hitT = tMax;
            int32_t id = -1;
            int32_t i = 0;
            {
                bool __need_loop_118 = true;
                das_iterator<TDim<struct Sphere,9>> __sph_iterator(das_global<TDim<struct Sphere,9>,352>(__context__) /*s_Spheres*/);
                struct Sphere * sph;
                __need_loop_118 = __sph_iterator.first(__context__,sph) && __need_loop_118;
                for ( ; __need_loop_118 ; __need_loop_118 = __sph_iterator.next(__context__,sph) )
                {
                    vec4f /*float3*/ co = (SimPolicy<float3>::Sub((*sph).center,r.orig,*__context__));
                    float nb = dot3(co,r.dir);
                    float discr = SimPolicy<float>::Mad(nb,nb,(*sph).radius2 - lengthSq3(co),*__context__);
                    if ( discr > 0.f )
                    {
                        float discrSq = SimPolicy<float>::Sqrt(discr,*__context__);
                        float t = (nb - discrSq);
                        if ( t <= tMin )
                        {
                            t = nb + discrSq;
                        };
                        if ( (t > tMin) & (t < hitT) )
                        {
                            id = i;
                            hitT = t;
                        };
                    };
                    ++i;
                }
                __sph_iterator.close(__context__,sph);
            };
            if ( id != -1 )
            {
                vec4f /*float3*/ atPos = pointAt(__context__,r,hitT);
                outHit = (([&]() -> struct Hit {
                    struct Hit __mks_133; das_zero(__mks_133);
                    __mks_133.pos = atPos;
                    __mks_133.normal = (SimPolicy<float3>::MulVecScal((SimPolicy<float3>::Sub(atPos,das_global<TDim<struct Sphere,9>,352>(__context__) /*s_Spheres*/(id,__context__).center,*__context__)),cast<float>::from(das_global<TDim<struct Sphere,9>,352>(__context__) /*s_Spheres*/(id,__context__).iRadius),*__context__));
                    __mks_133.t = hitT;
                    return __mks_133;
                })());
            };
            return id;
        }

        bool hitWorld ( Context * __context__, const struct Ray &  r, float tMin, float tMax, struct Hit &  outHit, int32_t & outID )
        {
            outID = hitSpheres(__context__,r,tMin,tMax,das_arg<struct Hit>::pass(outHit));
            return outID != -1;
        }

        int32_t job ( Context * __context__, TArray<float3> &  backbuffer, int32_t frameCount, int32_t width, int32_t height, int32_t ymin, int32_t ymax )
        {
            prepare(__context__);
            int32_t totalRayCount = 0;
            draw(__context__,0.f,frameCount,width,height,ymin,ymax,das_arg<TArray<float3>>::pass(backbuffer),totalRayCount);
            return totalRayCount;
        }

        float3 pointAt ( Context * __context__, const struct Ray &  ray, float t )
        {
            return SimPolicy<float3>::Add(ray.orig,(SimPolicy<float3>::MulVecScal(ray.dir,cast<float>::from(t),*__context__)),*__context__);
        }

        void prepare ( Context * __context__ )
        {
            {
                bool __need_loop_245 = true;
                das_iterator<TDim<struct Sphere,9>> __sph_iterator(das_global<TDim<struct Sphere,9>,352>(__context__) /*s_Spheres*/);
                struct Sphere * sph;
                __need_loop_245 = __sph_iterator.first(__context__,sph) && __need_loop_245;
                for ( ; __need_loop_245 ; __need_loop_245 = __sph_iterator.next(__context__,sph) )
                {
                    (*sph).radius2 = (*sph).radius * (*sph).radius;
                    (*sph).iRadius = 1.f / (*sph).radius;
                }
                __sph_iterator.close(__context__,sph);
            };
        }

        void resize ( Context * __context__, TArray<float3> &  Arr, int32_t newSize )
        {
            builtin_array_resize(das_arg<TArray<float3>>::pass(Arr),newSize,12,__context__);
        }

        bool scatter ( Context * __context__, const struct Material &  mat, const struct Ray &  r_in, struct Hit &  rec, float3 & attenuation, struct Ray &  scattered, float3 & outLightE, int32_t & inoutRayCount )
        {
            outLightE = float3(0.f,0.f,0.f);
            if ( mat.mtype == Type::Lambert )
            {
                vec4f /*float3*/ target = (SimPolicy<float3>::Add((SimPolicy<float3>::Add(rec.pos,rec.normal,*__context__)),randomUnitVector(das_global<int4,0>(__context__) /*RAND_SEED*/),*__context__));
                scattered = (([&]() -> struct Ray {
                    struct Ray __mks_146; das_zero(__mks_146);
                    __mks_146.orig = rec.pos;
                    __mks_146.dir = normalize4(SimPolicy<float3>::Sub(target,rec.pos,*__context__));
                    return __mks_146;
                })());
                attenuation = mat.albedo;
                struct Sphere * s = &(das_global<TDim<struct Sphere,9>,352>(__context__) /*s_Spheres*/(8,__context__));
                vec4f /*float3*/ sw = normalize4(SimPolicy<float3>::Sub((*s).center,rec.pos,*__context__));
                vec4f /*float3*/ su = normalize4(cross3((SimPolicy<float>::Abs(v_extract_x(sw) /*x*/,*__context__) > 0.00999999978f) ? float3(0.f,1.f,0.f) : float3(1.f,0.f,0.f),sw));
                vec4f /*float3*/ sv = cross3(sw,su);
                float cosAMax = SimPolicy<float>::Sqrt(SimPolicy<float>::Sat(1.f - (((*s).radius * (*s).radius) * invdistanceSq3(rec.pos,(*s).center)),*__context__),*__context__);
                vec4f /*float4*/ eps = randomFloat4(das_global<int4,0>(__context__) /*RAND_SEED*/);
                float cosA = (SimPolicy<float>::Mad(v_extract_x(eps) /*x*/,cosAMax,1.f,*__context__) - v_extract_x(eps) /*x*/);
                float sinA = SimPolicy<float>::Sqrt(1.f - (cosA * cosA),*__context__);
                float sinPhi = 0; float cosPhi = 0;
                sincosF(6.28318548f * v_extract_y(eps) /*y*/,sinPhi,cosPhi);
                vec4f /*float3*/ l = (SimPolicy<float3>::Add((SimPolicy<float3>::Add((SimPolicy<float3>::MulVecScal(su,cast<float>::from((cosPhi * sinA)),*__context__)),(SimPolicy<float3>::MulVecScal(sv,cast<float>::from((sinPhi * sinA)),*__context__)),*__context__)),(SimPolicy<float3>::MulVecScal(sw,cast<float>::from(cosA),*__context__)),*__context__));
                struct Hit lightHit; das_zero(lightHit);
                int32_t hitID = 0;
                ++inoutRayCount;
                struct Ray ray = (([&]() -> struct Ray {
                    struct Ray __mks_162; das_zero(__mks_162);
                    __mks_162.orig = rec.pos;
                    __mks_162.dir = l;
                    return __mks_162;
                })());
                if ( hitWorld(__context__,das_arg<struct Ray>::pass(ray),0.00100000005f,10000000.f,das_arg<struct Hit>::pass(lightHit),hitID) & (hitID == 8) )
                {
                    vec4f /*float3*/ nl = ((dot3(rec.normal,r_in.dir) < 0.f) ? vec4f(rec.normal) : SimPolicy<float3>::Unm(rec.normal,*__context__));
                    SimPolicy<float3>::SetAdd((char *)&(outLightE),(SimPolicy<float3>::MulVecScal((SimPolicy<float3>::Mul(mat.albedo,das_global<TDim<struct Material,9>,16>(__context__) /*s_SphereMats*/(8,__context__).emissive,*__context__)),cast<float>::from((SimPolicy<float>::Sat(dot3(l,nl),*__context__) * SimPolicy<float>::Mad(-2.f,cosAMax,2.f,*__context__))),*__context__)),*__context__);
                };
                return true;
            }    else
                if ( mat.mtype == Type::Metal )
                {
                    vec4f /*float3*/ refl = reflect(r_in.dir,rec.normal);
                    scattered = (([&]() -> struct Ray {
                        struct Ray __mks_169; das_zero(__mks_169);
                        __mks_169.orig = rec.pos;
                        __mks_169.dir = normalize4(SimPolicy<float3>::MadS(randomInUnitSphere(das_global<int4,0>(__context__) /*RAND_SEED*/),cast<float>::from(mat.roughness),refl,*__context__));
                        return __mks_169;
                    })());
                    attenuation = mat.albedo;
                    return dot3(scattered.dir,rec.normal) > 0.f;
                }    else
                    if ( mat.mtype == Type::Dielectric )
                    {
                        attenuation = float3(1.f,1.f,1.f);
                        vec4f /*float3*/ refr = v_zero();
                        float reflProb = 0;
                        if ( dot3(r_in.dir,rec.normal) > 0.f )
                        {
                            if ( refract(r_in.dir,SimPolicy<float3>::Unm(rec.normal,*__context__),mat.ri,cast_vec_ref<float3>(refr)) )
                            {
                                reflProb = schlick(__context__,mat.ri * dot3(r_in.dir,rec.normal),mat.ri);
                            }            else
                            {
                                reflProb = 1.f;
                            };
                        }        else
                        {
                            if ( refract(r_in.dir,rec.normal,1.f / mat.ri,cast_vec_ref<float3>(refr)) )
                            {
                                reflProb = schlick(__context__,-dot3(r_in.dir,rec.normal),mat.ri);
                            }            else
                            {
                                reflProb = 1.f;
                            };
                        };
                        if ( randomFloat(das_global<int4,0>(__context__) /*RAND_SEED*/) < reflProb )
                        {
                            scattered = (([&]() -> struct Ray {
                                struct Ray __mks_187; das_zero(__mks_187);
                                __mks_187.orig = rec.pos;
                                __mks_187.dir = normalize4(reflect(r_in.dir,rec.normal));
                                return __mks_187;
                            })());
                        }        else
                        {
                            scattered = (([&]() -> struct Ray {
                                struct Ray __mks_189; das_zero(__mks_189);
                                __mks_189.orig = rec.pos;
                                __mks_189.dir = normalize4(refr);
                                return __mks_189;
                            })());
                        };
                    }    else
                    {
                        attenuation = float3(1.f,0.f,1.f);
                        scattered = (([&]() -> struct Ray {
                            struct Ray __mks_192; das_zero(__mks_192);
                            __mks_192.orig = float3(0.f,0.f,0.f);
                            __mks_192.dir = float3(0.f,0.f,0.f);
                            return __mks_192;
                        })());
                        return false;
                    };
            return true;
        }

        float schlick ( Context * __context__, float cosine, float ri )
        {
            float r0 = ((1.f - ri) / (1.f + ri));
            r0 = r0 * r0;
            return SimPolicy<float>::Lerp(r0,1.f,SimPolicy<float>::Pow(1.f - cosine,5.f,*__context__),*__context__);
        }

        bool test ( Context * __context__ )
        {
            prepare(__context__);
            TArray<float3> backbuffer; das_zero(backbuffer);
            resize(__context__,das_arg<TArray<float3>>::pass(backbuffer),76800);
            float s = 0.f;
            int32_t totalRayCount = 0;
            builtin_print("\ntracing...\n",__context__);
            /* handled */ das::Time sw0 = /*c-tor*/ builtin_clock();
            {
                bool __need_loop_269 = true;
                das_iterator<range> __i_iterator(range(0,16));
                int32_t i;
                __need_loop_269 = __i_iterator.first(__context__,i) && __need_loop_269;
                for ( ; __need_loop_269 ; __need_loop_269 = __i_iterator.next(__context__,i) )
                {
                    int32_t rayCount = 0;
                    draw(__context__,s,i,320,240,0,240,das_arg<TArray<float3>>::pass(backbuffer),rayCount);
                    totalRayCount += rayCount;
                    /* handled */ das::Time sw1 = /*c-tor*/ builtin_clock();
                    s = SimPolicy<das::Time>::Sub(cast</* handled */ das::Time>::from(sw1),cast</* handled */ das::Time>::from(sw0),*__context__);
                    int32_t frames = (i + 1);
                    int32_t ms = int32_t((s / float(frames)) * 1000.f);
                    float mrayS = ((float(totalRayCount) / s) * 9.99999997e-07f);
                    float mrayFr = (float(rayCount) * 9.99999975e-06f);
                    builtin_print(das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_0, cast<int32_t>::from(ms), cast<char *>::from("ms\t"), cast<float>::from(mrayS), cast<char *>::from(" Mrays/s\t"), cast<float>::from(mrayFr), cast<char *>::from(" Mrays/frame\tframes "), cast<int32_t>::from(frames), cast<char *>::from("\n"))),__context__);
                }
                __i_iterator.close(__context__,i);
            };
            /* handled */ das::Time sw1 = /*c-tor*/ builtin_clock();
            builtin_print(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_1, cast<char *>::from("tracer took "), cast<float>::from((SimPolicy<das::Time>::Sub(cast</* handled */ das::Time>::from(sw1),cast</* handled */ das::Time>::from(sw0),*__context__))), cast<char *>::from(" sec\n"))),__context__);
            saveTga("path_tracer.tga",das_arg<TArray<float3>>::pass(backbuffer),320,240,__context__);
            return true;
        }

        float3 trace ( Context * __context__, struct Ray &  r, int32_t depth, int32_t & inoutRayCount, bool doMaterialE = true )
        {
            struct Hit rec; das_zero(rec);
            int32_t id = 0;
            ++inoutRayCount;
            if ( hitWorld(__context__,das_arg<struct Ray>::pass(r),0.00100000005f,10000000.f,das_arg<struct Hit>::pass(rec),id) )
            {
                struct Ray scattered; das_zero(scattered);
                vec4f /*float3*/ attenuation = v_zero();
                vec4f /*float3*/ lightE = v_zero();
                struct Material * mat = &(das_global<TDim<struct Material,9>,16>(__context__) /*s_SphereMats*/(id,__context__));
                vec4f /*float3*/ matE = (*mat).emissive;
                if ( (depth < 10) & scatter(__context__,das_arg<struct Material>::pass((*mat)),das_arg<struct Ray>::pass(r),das_arg<struct Hit>::pass(rec),cast_vec_ref<float3>(attenuation),das_arg<struct Ray>::pass(scattered),cast_vec_ref<float3>(lightE),inoutRayCount) )
                {
                    if ( !doMaterialE )
                    {
                        matE = float3(0.f,0.f,0.f);
                    };
                    doMaterialE = (*mat).mtype != Type::Lambert;
                    return SimPolicy<float3>::Mad(attenuation,trace(__context__,das_arg<struct Ray>::pass(scattered),depth + 1,inoutRayCount,doMaterialE),SimPolicy<float3>::Add(matE,lightE,*__context__),*__context__);
                }        else
                {
                    return matE;
                };
            }    else
            {
                vec4f /*float3*/ t = SimPolicy<float3>::MadS(r.dir,cast<float>::from(0.5f),float3(1.f,1.f,1.f),*__context__);
                return SimPolicy<float3>::Lerp(float3(0.300000012f,0.300000012f,0.300000012f),float3(0.150000006f,0.210000008f,0.300000012f),t,*__context__);
            };
        }

        int32_t trace ( Context * __context__, int32_t screenWidth, int32_t screenHeight, int32_t frameCount, int32_t ymin, int32_t ymax, TArray<float3> &  backbuffer, struct Camera &  cam )
        {
            vec4f /*float2*/ invWH = (SimPolicy<float2>::DivScalVec(cast<float>::from(1.f),float2(screenWidth,screenHeight),*__context__));
            vec4f /*float3*/ lerpFac = float3(float(frameCount) / float(frameCount + 1));
            int32_t rayCount = 0;
            int32_t backbufferIdx = (ymin * screenWidth);
            {
                bool __need_loop_222 = true;
                das_iterator<range> __y_iterator(range(ymin,ymax));
                int32_t y;
                __need_loop_222 = __y_iterator.first(__context__,y) && __need_loop_222;
                for ( ; __need_loop_222 ; __need_loop_222 = __y_iterator.next(__context__,y) )
                {
                    das_global<int4,0>(__context__) /*RAND_SEED*/ = randomSeed((y * 117) + (frameCount * 13));
                    {
                        bool __need_loop_224 = true;
                        das_iterator<range> __x_iterator(range(0,screenWidth));
                        int32_t x;
                        __need_loop_224 = __x_iterator.first(__context__,x) && __need_loop_224;
                        for ( ; __need_loop_224 ; __need_loop_224 = __x_iterator.next(__context__,x) )
                        {
                            vec4f /*float3*/ col = float3(0.f,0.f,0.f);
                            {
                                bool __need_loop_226 = true;
                                das_iterator<range> __s_iterator(range(0,4));
                                int32_t s;
                                __need_loop_226 = __s_iterator.first(__context__,s) && __need_loop_226;
                                for ( ; __need_loop_226 ; __need_loop_226 = __s_iterator.next(__context__,s) )
                                {
                                    vec4f /*float2*/ uv = (SimPolicy<float2>::Mul((SimPolicy<float2>::Add(float2(x,y),das_swizzle_seq<float2,float4,0>::swizzle(randomFloat4(das_global<int4,0>(__context__) /*RAND_SEED*/)) /*xy*/,*__context__)),invWH,*__context__));
                                    SimPolicy<float3>::SetAdd((char *)&(col),trace(__context__,das_arg<struct Ray>::pass(getRay(__context__,das_arg<struct Camera>::pass(cam),v_extract_x(uv) /*x*/,v_extract_y(uv) /*y*/)),0,rayCount,true),*__context__);
                                }
                                __s_iterator.close(__context__,s);
                            };
                            SimPolicy<float3>::SetMulScal((char *)&(col),cast<float>::from(0.25f),*__context__);
                            backbuffer(backbufferIdx,__context__) = SimPolicy<float3>::Lerp(col,backbuffer(backbufferIdx,__context__),lerpFac,*__context__);
                            backbufferIdx++;
                        }
                        __x_iterator.close(__context__,x);
                    };
                }
                __y_iterator.close(__context__,y);
            };
            return rayCount;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // Camera
            aotLib[0xaae51c191e7492f8] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_AotCMRES<struct Camera  ( Context * __context__, float3 lookFrom, float3 lookAt, float3 vup, float vfov, float aspect, float aperture, float focusDist ),Camera>>();
            };
            // draw
            aotLib[0x644efc0a9d87acd8] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, float time, int32_t frameCount, int32_t screenWidth, int32_t screenHeight, int32_t ymin, int32_t ymax, TArray<float3> & backbuffer, int32_t & outRayCount ),draw>>();
            };
            // getRay
            aotLib[0x8e8c1b89354a19cd] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_AotCMRES<struct Ray  ( Context * __context__, struct Camera & that, float s, float t ),getRay>>();
            };
            // hitSpheres
            aotLib[0xdf03989cc8379dd5] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<int32_t  ( Context * __context__, const struct Ray & r, float tMin, float tMax, struct Hit & outHit ),hitSpheres>>();
            };
            // hitWorld
            aotLib[0xbaa72e51caf8181b] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__, const struct Ray & r, float tMin, float tMax, struct Hit & outHit, int32_t & outID ),hitWorld>>();
            };
            // job
            aotLib[0xdb2e92eb5aed50cd] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<int32_t  ( Context * __context__, TArray<float3> & backbuffer, int32_t frameCount, int32_t width, int32_t height, int32_t ymin, int32_t ymax ),job>>();
            };
            // pointAt
            aotLib[0xd7679ef9bfcb0a99] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<float3  ( Context * __context__, const struct Ray & ray, float t ),pointAt>>();
            };
            // prepare
            aotLib[0xa353e08a7b4ddd] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__ ),prepare>>();
            };
            // resize
            aotLib[0xda78adcd97b8081b] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<float3> & Arr, int32_t newSize ),resize>>();
            };
            // scatter
            aotLib[0x4704a399b95ab7bb] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__, const struct Material & mat, const struct Ray & r_in, struct Hit & rec, float3 & attenuation, struct Ray & scattered, float3 & outLightE, int32_t & inoutRayCount ),scatter>>();
            };
            // schlick
            aotLib[0x4cefe96c46902f1f] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<float  ( Context * __context__, float cosine, float ri ),schlick>>();
            };
            // test
            aotLib[0x44970e159078c85e] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
            // trace
            aotLib[0xb0b93da2c15e556f] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<float3  ( Context * __context__, struct Ray & r, int32_t depth, int32_t & inoutRayCount, bool doMaterialE ),trace>>();
            };
            // trace
            aotLib[0x348bd8fefb63b247] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<int32_t  ( Context * __context__, int32_t screenWidth, int32_t screenHeight, int32_t frameCount, int32_t ymin, int32_t ymax, TArray<float3> & backbuffer, struct Camera & cam ),trace>>();
            };
            // [[ init script ]]
            aotLib[0x88f9131b111b2a64] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void (Context *), __init_script>>();
            };
        }
    }
}
