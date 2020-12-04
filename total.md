

# 이미지 텍스처 매핑

HP에서 **𝐏**피, 우리는 표면 좌표를 계산합니다 ( 𝑢 , 𝑣 )(유,V). 그런 다음이를 사용하여 절차 적 솔리드 텍스처 (대리석과 같은)를 색인화합니다. 이미지를 읽고 2D를 사용할 수도 있습니다.( 𝑢 , 𝑣 )(유,V) 이미지에 인덱스 할 텍스처 좌표입니다.



scaled를 사용하는 직접적인 방법 ( 𝑢 , 𝑣 )(유,V) 이미지에서 𝑢유 과 𝑣V 정수로 사용하고 다음과 같이 사용하십시오. ( 𝑖 , 𝑗 )(나는,제이)픽셀. 이미지 해상도를 변경할 때 코드를 변경할 필요가 없기 때문에 이것은 어색합니다. 따라서 그래픽에서 가장 보편적 인 비공식 표준 중 하나는 이미지 픽셀 좌표 대신 텍스처 좌표를 사용하는 것입니다. 이것들은 이미지에서 일부 형태의 분수 위치입니다. 예를 들어 픽셀의 경우( 𝑖 , 𝑗 )(나는,제이) 에 𝑁𝑥엔엑스 으로 𝑁𝑦엔와이 이미지에서 이미지 텍스처 위치는 다음과 같습니다.





𝑢 =𝑖𝑁𝑥− 1유=나는엔엑스−1



𝑣 =𝑗𝑁𝑦− 1V=제이엔와이−1





이것은 단지 분수 위치입니다.



## 텍스처 이미지 데이터 저장

이제 이미지를 포함하는 텍스처 클래스도 만들어야합니다. 내가 가장 좋아하는 이미지 유틸리티 [stb_image](https://github.com/nothings/stb) 를 사용할 것 입니다. 서명되지 않은 문자의 큰 배열로 이미지를 읽습니다. 이들은 [0,255] (검정에서 완전 흰색) 범위의 각 구성 요소가있는 패킹 된 RGB입니다. `image_texture`클래스 생성 된 화상 데이터를 사용한다.

```
#include "rtweekend.h"
#include "rtw_stb_image.h"
#include "perlin.h"

#include <iostream>

...

class image_texture : public texture {
    public:
        const static int bytes_per_pixel = 3;

        image_texture()
          : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

        image_texture(const char* filename) {
            auto components_per_pixel = bytes_per_pixel;

            data = stbi_load(
                filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                width = height = 0;
            }

            bytes_per_scanline = bytes_per_pixel * width;
        }

        ~image_texture() {
            delete data;
        }

        virtual color value(double u, double v, const vec3& p) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return color(0,1,1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = clamp(u, 0.0, 1.0);
            v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

            auto i = static_cast<int>(u * width);
            auto j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= width)  i = width-1;
            if (j >= height) j = height-1;

            const auto color_scale = 1.0 / 255.0;
            auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

            return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
        }

    private:
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;
};
```

**목록 44 :** [texture.h] 이미지 텍스처 클래스



그 순서로 패킹 된 배열의 표현은 꽤 표준입니다. 고맙게도 [stb_image](https://github.com/nothings/stb) 패키지를 사용하면 매우 간단 `rtw_stb_image.h`합니다. 컴파일러 경고도 처리 하는 헤더를 작성하면됩니다 .



```
#ifndef RTWEEKEND_STB_IMAGE_H
#define RTWEEKEND_STB_IMAGE_H

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
    // Microsoft Visual C++ Compiler
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

// Restore warning levels.
#ifdef _MSC_VER
    // Microsoft Visual C++ Compiler
    #pragma warning (pop)
#endif

#endif
```

**목록 45 :** [rtw_stb_image.h] stb_image 패키지 포함

위의 내용은를 `stb_image.h`라는 폴더에 복사했다고 가정합니다 `external`. 디렉토리 구조에 따라 조정하십시오.





## 이미지 텍스처 사용



방금 웹에서 임의의 지구지도를 가져 왔습니다. 모든 표준 투영이 우리의 목적에 적합합니다.





[![img](https://raytracing.github.io/images/earthmap.jpg)](https://raytracing.github.io/images/earthmap.jpg)*이미지 14 : earthmap.jpg*









다음은 파일에서 이미지를 읽어 디퓨즈 머티리얼에 할당하는 코드입니다.



```
hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}
```

**목록 46 :** [main.cc] stbi_load ()를 사용하여 이미지로드





우리는 모든 색상의 힘 중 일부가 텍스처 인 것을보기 시작합니다. 람베르 티안 재질에 어떤 종류의 텍스처도 할당 할 수 있으며 람베르 티안은이를 인식 할 필요가 없습니다.



이를 테스트하려면 main에 던지십시오.



```
int main() {
    ...
    switch (0) {
        ...
        default:        case 3:
            ...

        default:
        case 4:
            world = earth();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;    }
    ...
```

**목록 47 :** [main.cc] 매개 변수보기

사진이 중앙에 큰 청록색 구와 함께 돌아 오면 stb_image가 지구지도 사진을 찾지 못한 것입니다. 프로그램은 실행 파일과 동일한 디렉토리에서 파일을 찾습니다. 지구를 빌드 디렉토리에 복사하거나 `earth()`다른 곳을 가리 키도록 다시 작성 하세요.



[![img](https://raytracing.github.io/images/img-2.15-earth-sphere.png)](https://raytracing.github.io/images/img-2.15-earth-sphere.png)*이미지 15 : 지구 매핑 된 구*









# 직사각형 및 조명

조명은 레이트 레이싱의 핵심 구성 요소입니다. 초기의 단순한 레이트 레이서는 공간의 점이나 방향과 같은 추상 광원을 사용했습니다. 현대적인 접근 방식에는 위치와 크기가있는 물리적 기반 조명이 더 많이 있습니다. 이러한 광원을 생성하려면 일반 오브젝트를 장면에 빛을 방출하는 것으로 바꿀 수 있어야합니다.



## 이미 시브 머티리얼



먼저 발광 재료를 만들어 보겠습니다. 방출 된 함수를 추가해야합니다 ( `hit_record`대신에 추가 할 수도 있습니다 . 디자인 취향의 문제입니다). 배경과 마찬가지로 광선에 그것이 무엇인지 알려주고 반사를 수행하지 않습니다. 매우 간단합니다.



```
class diffuse_light : public material  {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            return false;
        }

        virtual color emitted(double u, double v, const point3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<texture> emit;
};
```

**목록 48 :** [material.h] 확산 조명 클래스







모든 비 방출 재료를 구현할 필요가 없도록 `emitted()`기본 클래스가 검은 색을 반환합니다.



```
class material {
    public:
        virtual color emitted(double u, double v, const point3& p) const {
            return color(0,0,0);
        }
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};
```

**목록 49 :** [material.h] 클래스 자료에서 새로 내 보낸 함수







## 광선 색상 기능에 배경색 추가



다음으로 순수한 검정색 배경을 원하므로 장면의 유일한 빛이 이미 터에서 나옵니다. 이를 위해 `ray_color`함수에 배경색 매개 변수를 추가 하고 새 `emitted`값에 주의를 기울 입니다.



```
color ray_color(const ray& r, const color& background, const hittable& world, int depth) {    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);


    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);}
...

int main() {
    ...

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    color background(0,0,0);
    switch (0) {
        case 1:
            world = random_scene();
            background = color(0.70, 0.80, 1.00);            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            break;

        case 2:
            world = two_spheres();
            background = color(0.70, 0.80, 1.00);            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;

        case 3:
            world = two_perlin_spheres();
            background = color(0.70, 0.80, 1.00);            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;


        default:        case 4:
            world = earth();
            background = color(0.70, 0.80, 1.00);            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;


        default:
        case 5:
            background = color(0.0, 0.0, 0.0);
            break;    }

    ...
                pixel_color += ray_color(r, background, world, max_depth);    ...
}
```

**목록 50 :** [main.cc] 재료 방출을위한 ray_color 함수

광선이 닿았을 때 하늘의 색상을 결정하는 데 사용한 코드를 제거하고 있으므로 이전 장면 렌더링에 대한 새 색상 값을 전달해야합니다. 우리는 하늘 전체에 평평한 청백색을 고수하기로 결정했습니다. 항상 부울을 전달하여 이전 스카이 박스 코드와 새로운 단색 배경 사이를 전환 할 수 있습니다. 여기서는 간단하게 유지합니다.







## 직사각형 개체 만들기

이제 직사각형을 만들어 보겠습니다. 직사각형은 종종 인공 환경을 모델링하는 데 편리합니다. 축 정렬 직사각형은 쉽기 때문에 팬입니다. (나중에 회전 할 수 있도록 인스턴스화 할 것입니다.)



먼저 xy 평면에 직사각형이 있습니다. 이러한 평면은 z 값으로 정의됩니다. 예를 들면𝑧 = 𝑘지=케이. 축 정렬 직사각형은 선으로 정의됩니다.𝑥 =𝑥0엑스=엑스0, 𝑥 =𝑥1엑스=엑스1, 𝑦 =𝑦0와이=와이0, 및 𝑦 =𝑦1와이=와이1.





[![img](https://raytracing.github.io/images/fig-2.05-ray-rect.jpg)](https://raytracing.github.io/images/fig-2.05-ray-rect.jpg)***그림 5 :** 광선-직사각형 교차*









광선이 이러한 직사각형에 닿는 지 여부를 확인하기 위해 먼저 광선이 평면에 닿는 위치를 결정합니다. 광선이**𝐏** (𝑡)= **𝐀** +𝑡 **𝐛**피(티)=ㅏ+티비 z 구성 요소는 다음과 같이 정의됩니다. 𝑃𝑧( 𝑡 ) =𝐴𝑧+ 𝑡𝑏𝑧피지(티)=ㅏ지+티비지. t가 어디에 있는지 풀 수있는 항을 재 배열하면𝑧 = 𝑘지=케이.





𝑡 =𝑘 −𝐴𝑧𝑏𝑧티=케이−ㅏ지비지







일단 우리가 𝑡티, 우리는 그것을 방정식에 대입 할 수 있습니다. 𝑥엑스 과 𝑦와이:





𝑥 =𝐴𝑥+ 𝑡𝑏𝑥엑스=ㅏ엑스+티비엑스



𝑦 =𝐴𝑦+ 𝑡𝑏𝑦와이=ㅏ와이+티비와이





다음과 같은 경우 히트입니다 𝑥0< 𝑥 <𝑥1엑스0<엑스<엑스1 과 𝑦0< 𝑦 <𝑦1와이0<와이<와이1.

직사각형은 축으로 정렬되어 있기 때문에 경계 상자는 무한히 얇은면을 갖게됩니다. 축 정렬 경계 볼륨 계층으로 분할 할 때 문제가 될 수 있습니다. 이에 대응하기 위해 모든 히트 가능 객체는 모든 차원을 따라 유한 너비를 갖는 경계 상자를 가져야합니다. 직사각형의 경우 무한히 얇은면에 상자를 약간 채 웁니다.



따라서 실제 `xy_rect`클래스는 다음과 같습니다.



```
#ifndef AARECT_H
#define AARECT_H

#include "rtweekend.h"

#include "hittable.h"

class xy_rect : public hittable {
    public:
        xy_rect() {}

        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k,
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = aabb(point3(x0,y0, k-0.0001), point3(x1, y1, k+0.0001));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, y0, y1, k;
};

#endif
```

**목록 51 :** [aarect.h] XY 평면 직사각형 객체







그리고 hit 함수는 다음과 같습니다.



```
bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}
```

**목록 52 :** [aarect.h] XY 직사각형 객체에 대한 적중 함수







## 물체를 빛으로 바꾸기



직사각형을 조명으로 설정하면 :



```
hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}
```

**목록 53 :** [main.cc] 간단한 직사각형 조명







그런 다음 새 장면을 만들고 배경색을 검정색으로 설정하는 데주의를 기울입니다.



```
#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"
#include "aarect.h"
#include <iostream>
...
int main() {
    ...
    switch (0) {
        ...
        default:
        case 5:
            world = simple_light();
            samples_per_pixel = 400;
            background = color(0,0,0);
            lookfrom = point3(26,3,6);
            lookat = point3(0,2,0);
            vfov = 20.0;
            break;    }
    ...
```

**목록 54 :** [main.cc] 검정색 배경의 직사각형 조명 장면



우리는 :





[![img](https://raytracing.github.io/images/img-2.16-rect-light.png)](https://raytracing.github.io/images/img-2.16-rect-light.png)*이미지 16 : 직사각형 광원이있는 장면*







빛이 ( 1 , 1 , 1 )(1,1,1). 이것은 사물을 비추기에 충분히 밝게합니다.



몇 가지 구체 조명을 만드는 것도 바보입니다.





[![img](https://raytracing.github.io/images/img-2.17-rect-sphere-light.png)](https://raytracing.github.io/images/img-2.17-rect-sphere-light.png)*이미지 17 : 직사각형 및 구형 광원이있는 장면*









## 더 많은 축 정렬 직사각형

이제 다른 두 개의 축과 유명한 Cornell Box를 추가하겠습니다.



이것은 xz 및 yz입니다.



```
class xz_rect : public hittable {
    public:
        xz_rect() {}

        xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Y
            // dimension a small amount.
            output_box = aabb(point3(x0,k-0.0001,z0), point3(x1, k+0.0001, z1));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, z0, z1, k;
};

class yz_rect : public hittable {
    public:
        yz_rect() {}

        yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
            shared_ptr<material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the X
            // dimension a small amount.
            output_box = aabb(point3(k-0.0001, y0, z0), point3(k+0.0001, y1, z1));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double y0, y1, z0, z1, k;
};
```

**목록 55 :** [aarect.h] XZ 및 YZ 직사각형 객체







놀라운 히트 기능으로 :



```
bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;
    auto y = r.origin().y() + t*r.direction().y();
    auto z = r.origin().z() + t*r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}
```

**목록 56 :** [aarect.h] XZ 및 YZ 직사각형 객체 적중 함수







## 빈 "코넬 상자"만들기



"Cornell Box"는 확산 표면 간의 빛 상호 작용을 모델링하기 위해 1984 년에 도입되었습니다. 5 개의 벽과 상자의 빛을 만들어 봅시다.



```
hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    return objects;
}
```

**목록 57 :** [main.cc] Cornell 상자 장면, 비어 있음







보기 및 장면 정보 추가 :



```
int main() {
    ...
    switch (0) {
        ...
        default:        case 5:
            ...
            break;


        default:
        case 6:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;    }
    ...
```

**목록 58 :** [main.cc] 종횡비 변경 및 매개 변수보기







우리는 :





[![img](https://raytracing.github.io/images/img-2.18-cornell-empty.png)](https://raytracing.github.io/images/img-2.18-cornell-empty.png)*이미지 18 : 빈 코넬 상자*



이 이미지는 빛이 작기 때문에 매우 시끄 럽습니다.





# 인스턴스

Cornell Box에는 일반적으로 두 개의 블록이 있습니다. 벽을 기준으로 회전합니다. 먼저, 6 개의 직사각형을 포함하는 축 정렬 블록 원형을 만들어 보겠습니다.

```
#ifndef BOX_H
#define BOX_H

#include "rtweekend.h"

#include "aarect.h"
#include "hittable_list.h"

class box : public hittable  {
    public:
        box() {}
        box(const point3& p0, const point3& p1, shared_ptr<material> ptr);

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = aabb(box_min, box_max);
            return true;
        }

    public:
        point3 box_min;
        point3 box_max;
        hittable_list sides;
};

box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    return sides.hit(r, t_min, t_max, rec);
}

#endif
```

**목록 59 :** [box.h] 상자 객체



이제 두 개의 블록을 추가 할 수 있습니다 (회전하지 않음).



```
#include "box.h"
...
objects.add(make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white));
objects.add(make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white));
```

**목록 60 :** [main.cc] 상자 객체 추가







이것은 다음을 제공합니다.





[![img](https://raytracing.github.io/images/img-2.19-cornell-blocks.png)](https://raytracing.github.io/images/img-2.19-cornell-blocks.png)*이미지 19 : 두 개의 블록이있는 코넬 상자*









이제 상자가 생겼으므로 *실제* Cornell 상자 와 일치하도록 약간 회전해야합니다 . 레이트 레이싱에서 이것은 일반적으로 *인스턴스로* 수행됩니다 . 인스턴스는 어떤 식 으로든 이동 또는 회전 된 기하학적 원형입니다. 이것은 우리가 아무것도 움직이지 않기 때문에 레이 트레이싱에서 특히 쉽습니다. 대신 우리는 광선을 반대 방향으로 이동합니다. 예를 들어 *번역* (종종 *이동* 이라고 함)을 생각해보십시오 . 원점에서 분홍색 상자를 가져 와서 모든 x 구성 요소에 2를 더하거나 (거의 항상 레이트 레이싱 에서처럼) 상자를 그대로 둡니다. 그러나 히트 루틴에서 x 구성 요소에서 2를 뺍니다. 레이 원점.





[![img](https://raytracing.github.io/images/fig-2.06-ray-box.jpg)](https://raytracing.github.io/images/fig-2.06-ray-box.jpg)***그림 6 :** 이동 된 광선 대 상자가있는 광선 상자 교차*









## 인스턴스 번역



이것을 움직임으로 생각하든 좌표 변경으로 생각하든 당신에게 달려 있습니다. 기본 적중 테이블을 이동하는 코드는 *번역* 인스턴스입니다.



```
class translate : public hittable {
    public:
        translate(shared_ptr<hittable> p, const vec3& displacement)
            : ptr(p), offset(displacement) {}

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<hittable> ptr;
        vec3 offset;
};

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool translate::bounding_box(double time0, double time1, aabb& output_box) const {
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;

    output_box = aabb(
        output_box.min() + offset,
        output_box.max() + offset);

    return true;
}
```

**목록 61 :** [hittable.h] Hittable 번역 클래스







## 인스턴스 회전



회전은 공식을 이해하거나 생성하는 것만 큼 쉽지 않습니다. 일반적인 그래픽 전술은 x, y 및 z 축에 대한 모든 회전을 적용하는 것입니다. 이러한 회전은 어떤 의미에서 축 정렬됩니다. 먼저 z 축을 중심으로 세타로 회전하겠습니다. 그것은 x와 y 만 변경하고 z에 의존하지 않는 방식으로 변경됩니다.





[![img](https://raytracing.github.io/images/fig-2.07-rot-z.jpg)](https://raytracing.github.io/images/fig-2.07-rot-z.jpg)***그림 7 :** Z 축을 중심으로 한 회전*









여기에는 여기서 다루지 않을 공식을 사용하는 몇 가지 기본 삼각법이 포함됩니다. 그것은 당신에게 그것이 약간 관련된 정확한 인상을 주지만 그것은 간단하며 어떤 그래픽 텍스트와 많은 강의 노트에서 찾을 수 있습니다. z에 대해 시계 반대 방향으로 회전 한 결과는 다음과 같습니다.





𝑥′= cos( 𝜃 ) ⋅ 𝑥 − 죄( 𝜃 ) ⋅ 𝑦엑스′=코사인⁡(θ)⋅엑스−죄⁡(θ)⋅와이



𝑦′= 죄( 𝜃 ) ⋅ 𝑥 + cos( 𝜃 ) ⋅ 𝑦와이′=죄⁡(θ)⋅엑스+코사인⁡(θ)⋅와이





가장 좋은 점은 𝜃θ사분면이나 그와 비슷한 케이스는 필요하지 않습니다. 역변환은 반대의 기하학적 연산입니다.− 𝜃−θ. 여기, 기억하세요코사인( 𝜃 ) = cos( − 𝜃 )코사인⁡(θ)=코사인⁡(−θ) 과 죄( − 𝜃 ) = − 죄( 𝜃 )죄⁡(−θ)=−죄⁡(θ), 그래서 공식은 매우 간단합니다.



마찬가지로, y를 중심으로 회전하는 경우 (상자의 블록에 대해 수행하려는 경우) 공식은 다음과 같습니다.





𝑥′= cos( 𝜃 ) ⋅ 𝑥 + 죄( 𝜃 ) ⋅ 𝑧엑스′=코사인⁡(θ)⋅엑스+죄⁡(θ)⋅지



𝑧′= − 죄( 𝜃 ) ⋅ 𝑥 + cos( 𝜃 ) ⋅ 𝑧지′=−죄⁡(θ)⋅엑스+코사인⁡(θ)⋅지



그리고 x 축에 대해 :



𝑦′= cos( 𝜃 ) ⋅ 𝑦 − 죄( 𝜃 ) ⋅ 𝑧와이′=코사인⁡(θ)⋅와이−죄⁡(θ)⋅지



𝑧′= 죄( 𝜃 ) ⋅ 𝑦 + cos( 𝜃 ) ⋅ 𝑧지′=죄⁡(θ)⋅와이+코사인⁡(θ)⋅지





변환이있는 상황과 달리 표면 법선 벡터도 변경되므로 히트를 받으면 방향도 변환해야합니다. 다행스럽게도 회전의 경우 동일한 공식이 적용됩니다. 저울을 추가하면 상황이 더 복잡해집니다. 이에 대한 링크는 https://in1weekend.blogspot.com/ 웹 페이지를 참조 [하십시오](https://in1weekend.blogspot.com/) .



y 회전 클래스의 경우 다음이 있습니다.



```
class rotate_y : public hittable {
    public:
        rotate_y(shared_ptr<hittable> p, double angle);

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = bbox;
            return hasbox;
        }

    public:
        shared_ptr<hittable> ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        aabb bbox;
};
```

**목록 62 :** [hittable.h] Hittable rotate-Y 클래스







생성자 사용 :



```
rotate_y::rotate_y(shared_ptr<hittable> p, double angle) : ptr(p) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    point3 min( infinity,  infinity,  infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                auto newx =  cos_theta*x + sin_theta*z;
                auto newz = -sin_theta*x + cos_theta*z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}
```

**목록 63 :** [hittable.h] Rotate-Y rotate 메서드







그리고 hit 함수 :



```
bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

    direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] =  cos_theta*rec.p[0] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

    normal[0] =  cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}
```

**목록 64 :** [hittable.h] Hittable Y-rotate hit 함수







그리고 Cornell의 변경 사항은 다음과 같습니다.



```
shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
box1 = make_shared<rotate_y>(box1, 15);
box1 = make_shared<translate>(box1, vec3(265,0,295));
objects.add(box1);

shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
box2 = make_shared<rotate_y>(box2, -18);
box2 = make_shared<translate>(box2, vec3(130,0,65));
objects.add(box2);
```

**목록 65 :** [main.cc] Y 회전 상자가있는 Cornell 장면







결과 :





[![img](https://raytracing.github.io/images/img-2.20-cornell-standard.png)](https://raytracing.github.io/images/img-2.20-cornell-standard.png)*이미지 20 : 표준 Cornell 상자 장면*









# 볼륨

레이트 레이서에 추가하는 것이 좋은 점은 연기 / 안개 / 안개입니다. 이를 *볼륨* 또는 *참여 미디어* 라고도 *합니다* . 추가하기 좋은 또 다른 기능은 서브 서피스 스 캐터링인데, 이는 오브젝트 내부의 짙은 안개와 같습니다. 볼륨은 표면과 다른 동물이기 때문에 일반적으로 소프트웨어 아키텍처의 혼란을 추가하지만, 귀여운 기술은 볼륨을 임의의 표면으로 만드는 것입니다. 연기가 볼륨의 모든 지점에있을 수도 있고 없을 수도있는 표면으로 대체 될 수 있습니다. 이것은 코드를 볼 때 더 의미가 있습니다.



## 일정한 밀도 매체

먼저 일정한 밀도의 부피로 시작하겠습니다. 거기를 통과하는 광선은 볼륨 내부에서 흩어 지거나 그림의 중간 광선처럼 끝까지 통과 할 수 있습니다. 옅은 안개와 같이 더 얇은 투명 볼륨은 중간과 같은 광선을 가질 가능성이 높습니다. 광선이 볼륨을 통과해야하는 거리는 광선이 통과 할 가능성도 결정합니다.



[![img](https://raytracing.github.io/images/fig-2.08-ray-vol.jpg)](https://raytracing.github.io/images/fig-2.08-ray-vol.jpg)***그림 8 :** 광선 볼륨 상호 작용*





광선이 볼륨을 통과하면 어느 지점에서나 흩어질 수 있습니다. 볼륨이 밀도가 높을수록 그럴 가능성이 높습니다. 광선이 작은 거리에서 산란 될 확률Δ 𝐿Δ엘 is :





확률 = 𝐶⋅ Δ 𝐿개연성=씨⋅Δ엘







어디 𝐶씨부피의 광학 밀도에 비례합니다. 모든 미분 방정식을 통해 난수에 대해 산란이 발생하는 거리를 얻습니다. 해당 거리가 볼륨을 벗어나면 "히트"가 없습니다. 일정한 부피를 위해서는 밀도가 필요합니다.𝐶씨그리고 경계. 경계에 다른 히트 테이블을 사용하겠습니다. 결과 클래스는 다음과 같습니다.



```
#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "rtweekend.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable {
    public:
        constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a)
            : boundary(b),
              neg_inv_density(-1/d),
              phase_function(make_shared<isotropic>(a))
            {}

        constant_medium(shared_ptr<hittable> b, double d, color c)
            : boundary(b),
              neg_inv_density(-1/d),
              phase_function(make_shared<isotropic>(c))
            {}

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            return boundary->bounding_box(time0, time1, output_box);
        }

    public:
        shared_ptr<hittable> boundary;
        shared_ptr<material> phase_function;
        double neg_inv_density;
};

#endif
```

**목록 66 :** [constant_medium.h] 상수 중간 등급







등방성의 산란 함수는 균일 한 임의 방향을 선택합니다.



```
class isotropic : public material {
    public:
        isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
        isotropic(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
};
```

**목록 67 :** [material.h] 등방성 클래스







그리고 hit 함수는 다음과 같습니다.



```
bool constant_medium::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;

    hit_record rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t+0.0001, infinity, rec2))
        return false;

    if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " <<  hit_distance << '\n'
                  << "rec.t = " <<  rec.t << '\n'
                  << "rec.p = " <<  rec.p << '\n';
    }

    rec.normal = vec3(1,0,0);  // arbitrary
    rec.front_face = true;     // also arbitrary
    rec.mat_ptr = phase_function;

    return true;
}
```

**목록 68 :** [constant_medium.h] 일정한 중간 적중 방법





경계 주변의 논리에 대해주의해야하는 이유는 이것이 볼륨 내부의 광선 원점에 대해 작동하는지 확인해야하기 때문입니다. 구름에서는 사물이 많이 튀어 나오므로 일반적인 경우입니다.

또한 위의 코드는 광선이 일정한 중간 경계를 벗어나면 경계 밖에서 영원히 계속 될 것이라고 가정합니다. 달리 말하면 경계 모양이 볼록하다고 가정합니다. 따라서이 특정 구현은 상자 또는 구와 같은 경계에는 작동하지만 공극이 포함 된 토러스 또는 모양에는 작동하지 않습니다. 임의의 모양을 처리하는 구현을 작성할 수 있지만 독자를위한 연습으로 남겨 두겠습니다.



## 스모크 박스와 포그 박스가있는 코넬 박스 렌더링



더 빠른 수렴을 위해 두 블록을 연기와 안개 (어둡고 밝은 입자)로 바꾸고 빛을 더 크게 (그리고 장면이 날아 가지 않도록 어둡게) 만드는 경우 :



```
#include "constant_medium.h"
...

hittable_list cornell_smoke() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    return objects;
}

...

int main() {
    ...
    switch (0) {
        ...
        default:
        case 7:
            world = cornell_smoke();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
    ...
}
```

**목록 69 :** [main.cc] 연기가있는 코넬 상자







우리는 :





[![img](https://raytracing.github.io/images/img-2.21-cornell-smoke.png)](https://raytracing.github.io/images/img-2.21-cornell-smoke.png)*이미지 21 : 연기 블록이있는 코넬 상자*









# 모든 새로운 기능을 테스트하는 장면

모든 것을 덮는 크고 얇은 안개와 파란색 표면 아래 반사 구 (우리는 명시 적으로 구현하지 않았지만 유전체 내부의 볼륨이 표면 아래 재료가되는 것입니다)와 함께 모든 것을 종합 해 보겠습니다. 렌더러에 남아있는 가장 큰 한계는 그림자 광선이 없다는 것입니다. 이것이 우리가 무료로 화선과 표면을 얻는 이유입니다. 양날의 디자인 결정입니다.

```
...
#include "bvh.h"
...

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
            vec3(-100,270,395)
        )
    );

    return objects;
}

int main() {
    ...
    switch (0) {
        ...
        default:
        case 8:
            world = final_scene();
            aspect_ratio = 1.0;
            image_width = 800;
            samples_per_pixel = 10000;
            background = color(0,0,0);
            lookfrom = point3(478, 278, -600);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
    ...
}
```

**목록 70 :** [main.cc] 최종 장면



픽셀 당 10,000 개의 광선으로 실행하면 다음과 같은 결과가 나타납니다.





[![img](https://raytracing.github.io/images/img-2.22-book2-final.jpg)](https://raytracing.github.io/images/img-2.22-book2-final.jpg)*이미지 22 : 최종 장면*







이제 나가서 자신의 정말 멋진 이미지를 만드십시오! 추가 읽기 및 기능에 대한 포인터는 https://in1weekend.blogspot.com/ 을 참조 하고 [ptrshrl@gmail.com](mailto:ptrshrl@gmail.com.) 에서 질문, 댓글 및 멋진 이미지를 이메일로 보내 [주십시오.](mailto:ptrshrl@gmail.com.)