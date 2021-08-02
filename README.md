# Composure용 Shadow 만들기

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/ComposureShadow.png)

빨간 박스 영역에 보이는 것처럼 Composure에서 평면 그림자뿐만 아니라 평면이 아닌 곳에도 그림자를 표현 할 수 있도록 하는 방법에 대해 정리 하고자 한다.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Composure에서 평면 그림자 표현

Unreal Engine 4의 Composure 샘플에서 기본적으로 평평한 바닥에 평면 그림자를 드리우게 할 수 있다. 그 원리에 대해 설명 하고자 한다.

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/PlaneShadow.png)

### 원리

바닥에 Shadow가 드리워진 Scene을 캡쳐한 이미지와 바닥에 Shadow가 드리워지지 않은 Scene을 캡쳐한 이미지를 얻어 이 두 이미지에서 Shadow Color를 추출해서 최종 출력에 합성 시켜주면 된다.

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/CreateShadowMask.png)

[그림: M_FinalCompositing Post-Process Material]

위 그림에서 빨간색으로 표시한 것이 Shadow를 추출하기 위해 계산하는 것으로, 둘 다 색이 같은 값이면 1이되고, Shadow Color 값이 있는 곳에서 추출한 픽셀 색은 Shadow Color / 1.0이 되어 Shadow Color 값을 얻게 된다.

노란색 박스로 표시한 부분은 추출한 Shadow Color 값을 좀 더 진하게 표현 될 수 있도록 하기 위해 Shadow Color 값에 2승 하는 거듭제곱 연산을 수행한다.

그럼 바닥에 Shadow가 드리워진 Scene을 어떻게 캡쳐 하는가?

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/ShadowCapture.png)

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/ShadowDetail.png)

**BP_CgCaptureCompElement** Actor를 Level에 배치 하고, 캡쳐하려고 하는 Actor를 지정 해 주면 된다. 캡쳐하려고 하는 Actor는 Composure의 Input의 Capture Actors에 추가 하면 되는데, Shadow가 드리워지는 Scene은 위 그림처럼 **Mannequin, GroundLayer, SkySphereLayer** 이렇게 3개를 설정 하였다.

GroundLayer는 바닥에 Shadow가 드리워지기 위해 바닥면이 있어야 하기 때문에 추가한다. 그리고 **UE4에서 Scene Capture시 사용하는 Render Target은 Clear 색이 검은색(0,0,0,1)으로 Clear 되도록 강제화(하드코딩) 되어있어** 나누기 연산을 하기 위해 배경이 흰색이어야 하기 때문에 Scene 전체를 덮는 SkySphereLayer도 추가한다.

Shadow가 드리워지지 않은 Scene 캡쳐는 **GroundLayer만 빠지고** 위와 같은 설정으로 BP_CgCaptureCompElement Actor를 설정하고 Level에 배치하면 된다.**(아래 그림 참조)**

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/NoShadowCapture.png)

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/NoShadowDetail.png)

Level에 배치 해 둔 **CompositingElement** Actor의 Composure -> Input -> Transform/Compositing Passes -> Transform Passes 항목의 입력 엘리먼트에 Post-Process Material에서 설정 한 항목에 Shadow가 드리워진, Shadow가 드리워지지 않은 Scene 캡쳐 정보를 전달 하기 위해 아래 그림의 빨간 박스로 표시된 것처럼 설정 해 준다.

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/CompositingElement.png)

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



