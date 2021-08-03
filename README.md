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

* Scene Capture시 Render Target을 검은색으로 Clear 하는 부분에 대한 설명.

  > BP_CgCaptureCompElement Actor는 내부에 SceneCaptureComponent2D를 사용해서 Scene Capture하는데, SceneCaptureComponent2D는 실제 Scene Capture를 FScene의 **UpdateScenCaptureContents()** 함수를 통해서 한다.
  >
  > UpdateSceneCaptureContents() 함수 구현 내용을 찾아 확인 해 보면 Render Target Clear를 수행 하는 Render Thread에서 실행 되는 코드에서 하도록 되어져 있으며, 그 코드가 존재하는 부분은 FScene의 **UpdateSceneCaptureContentDeferred_RenderThread()** 함수에 있다.

  ![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/UpdateSceneCaptures.png)

  [그림: FScene의 UpdateSceneCaptureContents 함수에서]

  ![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/UpdateSceneCaptureContent_RenderThread.png)

  [그림: FScene의 UpdateSceneCaptureContent_RenderThread 함수에서]

  ![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/UpdateSceneCaptureContentDeferred_RenderThread.png)

  [그림: FScene의 UpdateSceneCaptureContentDeferred_RenderThread 함수에서]

  > UpdateSceneCaptureContentDeferred_RenderThread() 함수 그림에서 빨간 박스로 표시된 내용을 보면 **FLinearColor::Black**으로 설정 하는 것을 볼 수 있다. 검정색으로 하드 코딩 되어져 있는 것이다.

Shadow가 드리워지지 않은 Scene 캡쳐는 **GroundLayer만 빠지고** 위와 같은 설정으로 BP_CgCaptureCompElement Actor를 설정하고 Level에 배치하면 된다.**(아래 그림 참조)**

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/NoShadowCapture.png)

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/NoShaodwDetail.png)

Level에 배치 해 둔 **CompositingElement** Actor의 Composure -> Input -> Transform/Compositing Passes -> Transform Passes 항목의 입력 엘리먼트에 Post-Process Material에서 설정 한 항목에 Shadow가 드리워진, Shadow가 드리워지지 않은 Scene 캡쳐 정보를 전달 하기 위해 아래 그림의 빨간 박스로 표시된 것처럼 설정 해 준다.

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/CompositingElement.png)

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Composure에서 바닥이 아닌 곳에 그림자 표현

Composure에서 바닥면이 아닌 벽면이나 계단과 같은 곳에 그림자를 드리우기 위해서 어떻게 해야 하는지에 대해 설명하고자 한다.

### 과제

기본 원리는 바닥에 Shadow를 드리우는 방식과 같다. 다만, 벽면이나 계단에 Shadow를 드리우기 위해서는 Diffuse Light에 따른 면의 음영에 대한 처리를 해 줘야 한다.

### 구현 1

벽과 같은 사각 물체(Cube Actor)를 Shadow가 드리워지는 Scene 캡쳐 항목에 추가하고, 또한 Shadow가 드리워지지 않는 Scene 캡쳐에도 추가 한다. 그리고 결과를 확인 해 보면 다음 그림과 같이 제대로 된 결과가 아니다. 기존의 Shadow 영역을 추출하는 방식으로는 바닥이 아니 벽면이나 계단같은 물체에 드리워진 Shadow를 제대로 추출하기가 힘들다.

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/Failed_1.png)

[그림: Shadow 결과 화면]

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/ShadowCaptureCube_1.png)

[그림: Shadow Scene 캡쳐 화면]

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/NoShadowCaptureCube_1.png)

[그림: No Shadow Scene 캡쳐 화면]

### 구현 2

Shadow가 드리워지는 Scene 캡쳐 하는 구성과 Shadow가 드리워지지 않는 Scene 캡쳐 구성을 똑같이 하고, Shadow가 드리워지지 않는 Scene 캡쳐에서 캐릭터의 Self Shadow만 나오게 캐릭터 설정을 변경하며 바닥과 벽과 같은 사각 물체(Cube Actor)에 Shadow가 생기지 않게 된다. 이렇게 한 상태에서 CompositingElement Actor에 설정된 Post-Process Material을 사용해서 Shadow를 추출하면 그림과 같이 정상적인 결과가 나온다.

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/Success_1.png)

[그림: Shadow 결과 화면]

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/ShadowCaptureCube_1.png)

[그림: Shadow Scene 캡쳐 화면]

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/NoShadowCaptureCube_2.png)

[그림: No Shadow Scene 캡쳐 화면 - 배경을 흰색으로 처리하는 SkySphereLayer 요소를 뺀 화면임.]

![](https://github.com/Devcoder-IndieWorks/ComposureShadow/blob/master/Images/NoShadowDetail_2.png)

[그림: No Shadow Scene 캡쳐 구성 요소]







