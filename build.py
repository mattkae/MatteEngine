import os
import pathlib
import glob

def runCmake(projectName, directory, vcpkgDir):
    print('Running cmake in directory {}'.format(directory))
    originalDir = os.getcwd()
    os.chdir(directory)

    fileList = []
    for root, dirs, files in os.walk('.'):
        if root.startswith('./build'):
            continue
        
        for filename in files:
            if filename.endswith('.h') or filename.endswith('.cpp'):
                if root == '.':
                    fileList.append(filename)
                elif root.startswith('./'):
                    folderName = root[root.find('/') + 1:]
                    fileList.append(folderName + '/' + filename)

    outContent = []
    cmakeFile = open('./CMakeLists.txt', 'r')
    for line in cmakeFile.readlines():
        line = line.strip()
        if len(line) == 0 or line == '\n':
            pass
        elif line.startswith('add_executable'):
            execStr = 'add_executable({}'.format(projectName)
            for filename in fileList:
                execStr = execStr + ' ' + filename
            execStr = execStr + ')'
            outContent.append(execStr)
        else:
            outContent.append(line)

    print(outContent)
    cmakeFile.close()
    cmakeFile = open('./CMakeLists.txt', 'w')
    cmakeFile.write('\n'.join(outContent))
    cmakeFile.close()
            

    if not os.path.exists('./build'):
        os.mkdir('./build')

    os.chdir('./build')

    os.system('cmake  "-DCMAKE_TOOLCHAIN_FILE={}" ../'.format(vcpkgDir))
    os.system('make')
    os.chdir(originalDir)

    print('Successfully build project')

def main():
    dirName = pathlib.Path(__file__).parent.absolute().as_posix()
    vcpkgDir = dirName + '/vcpkg/scripts/buildsystems/vcpkg.cmake'
    print('Executing the default initialization script')

    os.chdir(dirName)

    if not os.path.exists(vcpkgDir):
        print('TODO: Install vcpkg and all dependencies')
        pass

    runCmake('Matte', './src', vcpkgDir)
    runCmake('ModelTransformTool', './tool', vcpkgDir)

if __name__ == "__main__":
    main()
