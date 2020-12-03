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


---
# next week

## 3 bvh

aabb 가 어떻게 점 두개로 표현되지 박스인데?

박스는 양쪽 끝 대각선 두개면 표현이 되버리네!?

마지막 bvh 트리 생성은
생성자함수를 통해서 한방에 진행한다.
로직

1. 축을 무작위로 선택
2. 기본 요소 정렬 ..
3. 각 하위 트리 정렬

축 생성 ( 뭔가 x / y / z 를 무작위로 선택해서 계속 퀵소트를 돌리면 결국 된다 이거인건가..)
start랑, end로 요소 인덱스 시작 끝을 정해서 배열화?
object_span이 개수 인듯
1개 이면 걍 left = right = object[start] 로 하고 끝!
2개 면 왼쪽 오른쪽 비교해서 집어넣고
3개 이상이면 재귀로 소트 실행..

! 들어온 오브젝트 포인터를  bvh, hittable list 인지 구분해서 동작하게 짜야함.
bounding_box 의 첫번 째 인자 hittable list 말고 void *로 해서 분기를 줘야 할듯!. 