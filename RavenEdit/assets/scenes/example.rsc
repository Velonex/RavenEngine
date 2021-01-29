Scene: Example Scene
ClearColor: [0.392156869, 0.392156869, 0.392156869, 1]
Entities:
  - RUID: 545064385637976591
    TagComponent:
      Tag: PlusHorizontal
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 1.57079637]
      Scale: [1, 3, 1]
    SpriteRendererComponent:
      Color: [0.0784313753, 0.196078435, 0.588235319, 1]
  - RUID: 545064382883825923
    TagComponent:
      Tag: Cam
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Primary: true
      FixedAspectRatio: false
      Camera:
        ProjectionType: 1
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 10000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
  - RUID: 545061814968847908
    TagComponent:
      Tag: PlusVertical
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 3, 1]
    SpriteRendererComponent:
      Color: [0.0784313753, 0.196078435, 0.588235319, 1]