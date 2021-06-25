Scene: Fake Cube
ClearColor: [0.699999988, 0.699999988, 0.699999988, 1]
Entities:
  - RUID: 545330149665280940
    TagComponent:
      Tag: Left
    TransformComponent:
      Translation: [-0.349999994, 0, 0.349999994]
      Rotation: [0, -0.785398185, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 0, 0.949019611, 1]
      TextureID: 0
      TilingFactor: 1
  - RUID: 545330157860033757
    TagComponent:
      Tag: Right
    TransformComponent:
      Translation: [0.354999989, 0, 0.349999994]
      Rotation: [0, 0.785398185, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0.549019635, 0, 0.588235319, 1]
      TextureID: 0
      TilingFactor: 1
  - RUID: 545330277833052488
    TagComponent:
      Tag: Camera
    TransformComponent:
      Translation: [0, 1.70000005, 4]
      Rotation: [-0.404916406, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Primary: true
      FixedAspectRatio: false
      Camera:
        ProjectionType: 0
        PerspectiveFOV: 0.52359879
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 10000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
  - RUID: 545330143819731504
    TagComponent:
      Tag: Top
    TransformComponent:
      Translation: [0, 0.5, 0]
      Rotation: [1.57079637, 0.785398185, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0.874509811, 0, 0.835294127, 1]
      TextureID: 0
      TilingFactor: 1