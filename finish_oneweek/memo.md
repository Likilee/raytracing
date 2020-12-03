# 7 안티 앨리어싱

## 로직
픽샐 안에서 난수 발생을 통해 미세하게 다른 방향의 여러개의 ray(sample_per_pixel개)를 쏴서 얻은 색상을 pixel_color에 더해 준뒤, write color 할 때 sample_per_pixel 로 rgb를 나눠서 최종 색상을 결정한다.

### 난수 발생은 어떻게 할것인가?

rand() 함수 사용
**보너스**파트 이기 때문에 stdlib.h에 있는 rand사용 가능

# 8 확산 물체

1week 에서는 적분으로 diffuse 광자의 합을 구하지 않는데 이것은
7단원에서 안티 앨리어싱에서 픽셀당 샘플 평균을 하기 때문에 각각의 샘플합을 통해 적분을 진행하는 것과 유사한 효과를 내는 것 같다.
> 8은 diffuse meterial diffuse reflect와 같은 원리를 사용하지만 이 챕터는 확산 물질

[램버시안 반사](https://iskim3068.tistory.com/75)

# 10. 굴절

[스넬의 법칙](https://m.blog.naver.com/wa1998/221797520878)
