

Colors = {
    Color_White = 0,
    Color_Yellow = 1,
    Color_Red = 2,
    Color_Blue = 3,
    Color_Brown = 4,
 }

function LoadLevel1()
   
    --Level 1 --------------------------------------------------------------------------------------------
    --Create map floor
    CreateStaticCube(0.0, 0.0, 0.0,20.0, 1.0, 10.0, Colors.Color_White) 
    CreateStaticCube(0.0, 0.0, -5.5,18.0, 1.0, 1.0, Colors.Color_White) 
    CreateStaticCube(0.0, 0.0, -11.0,20.0, 1.0, 10.0, Colors.Color_White) 
    --Sides
    CreateStaticCube(10.5, 1.0, -5.5,1.0, 1.0, 21.0, Colors.Color_White)
    CreateStaticCube(-10.5, 1.0, -5.5,1.0, 1.0, 21.0, Colors.Color_White)
    -- Back
    CreateStaticCube(0.0, 1.0,5.5,22.0, 1.0, 1.0, Colors.Color_White)
    --Front
    CreateStaticCube(9.0, 1.0, -15.5, 2.0, 1.0, 1.0, Colors.Color_White)
    CreateStaticCube(-9.0, 1.0,-15.5, 2.0, 1.0, 1.0, Colors.Color_White)
    --Middle
    CreateStaticCube(0.0, 1.0, -5.5, 1.0, 1.0, 21.0, Colors.Color_White)

    --Blocks
    CreateStaticCube(5.5, 1.0, -6.5, 1.0, 1.0, 1.0, Colors.Color_Brown)
    CreateStaticCube(-5.5, 1.0, -6.5, 1.0, 1.0, 1.0, Colors.Color_Brown)

    --Detectors
    CreateDetector(9.5, 0.0, -5.5, 1.0, 1.0, 1.0, Colors.Color_Red)
    CreateDetector(-9.5, 0.0, -5.5, 1.0, 1.0, 1.0, Colors.Color_Red)

    --MovingCubes
    local cube_1_index = CreateMovingCube(5.5, 1.0, -3.0,Colors.Color_Red)
    local cube_2_index = CreateMovingCube(-5.5, 1.0, -3.0, Colors.Color_Red)

    --Doors
    local door1_index = CreateDoor(0, 3.0, -15.5, 16, 5.0, 1.0)

    SetDoorMovingBlocks(door1_index,cube_1_index,cube_2_index)

    SetPlayerPositions(3.0,1.5,2.5,-3.0,1.5,2.5)

 end


 function LoadLevel2()
   
   --Level 1 --------------------------------------------------------------------------------------------
    --Create map floor
    CreateStaticCube(0.0, 0.0, -21.0,20.0, 1.0, 10.0, Colors.Color_White) 
    CreateStaticCube(0.0, 0.0, -26.5,18.0, 1.0, 1.0, Colors.Color_White) 
    CreateStaticCube(0.0, 0.0, -32.0,20.0, 1.0, 10.0, Colors.Color_White) 
    --Sides
    CreateStaticCube(10.5, 1.0, -26.5,1.0, 1.0, 21.0, Colors.Color_White)
    CreateStaticCube(-10.5, 1.0, -26.5,1.0, 1.0, 21.0, Colors.Color_White)
    -- Back
    CreateStaticCube(0.0, 1.0,-15.5,22.0, 1.0, 1.0, Colors.Color_White)
    --Front
    CreateStaticCube(9.0, 1.0, -36.5, 2.0, 1.0, 1.0, Colors.Color_White)
    CreateStaticCube(-9.0, 1.0,-36.5, 2.0, 1.0, 1.0, Colors.Color_White)
    --Middle
    CreateStaticCube(0.0, 1.0, -20.5, 1.0, 1.0, 9.0, Colors.Color_White)
    CreateStaticCube(0.0, 2.0, -26.5, 1.0, 1.0, 5.0, Colors.Color_White)
    CreateStaticCube(0.0, 1.0, -32.5, 1.0, 1.0, 9.0, Colors.Color_White)

    --Blocks
    CreateStaticCube(-5, 1.0, -27.5, 1.0, 1.0, 1.0, Colors.Color_Brown)
    CreateStaticCube(3, 1.0, -27.5, 1.0, 1.0, 1.0, Colors.Color_Brown)

    --Detectors
    CreateDetector(9.5, 0.0, -26.5, 1.0, 1.0, 1.0, Colors.Color_Blue)
    CreateDetector(-9.5, 0.0, -26.5, 1.0, 1.0, 1.0, Colors.Color_Red)

    --MovingCubes
    local cube_1_index = CreateMovingCube(3, 1.0, -21.0,Colors.Color_Red)
    local cube_2_index = CreateMovingCube(-5, 1.0, -21.0, Colors.Color_Blue)

    --Doors
    local door1_index = CreateDoor(0, 3.0, -36.5, 16, 5.0, 1.0)

    SetDoorMovingBlocks(door1_index,cube_1_index,cube_2_index)

    SetPlayerPositions(7.5,1.5,-18,-7.5,1.5,-18)
 
 end