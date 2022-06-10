import os
import sys
import subprocess
import PyQt5
import dolphin_config
import mextk_config
import MainWindow
import configparser
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QWidget, QInputDialog, QLineEdit, QFileDialog, QMessageBox
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QProcess, QThread
from MainWindow import Ui_MainWindow
from configparser import ConfigParser

#Object for storing config variables(those each have a UI counterpart)
class configvars(object):
    ftCScriptPath = ""
    ftFunctionChkBox = False
    ftFunctionPath = ""
    ftDATChkBox = True
    ftDATPath = ""
    itCScriptPath = ""
    itID = 0
    itFunctionChkBox = False
    itFunctionPath = ""
    itDATChkBox = True
    itDATPath = ""
    grCScriptPath = ""
    grFunctionChkBox = False
    grFunctionPath = ""
    grDATPath = ""
    DolphinPath = ""
    FSdolPath = ""
    Log = ""
    Debug = ""
    MexTKPath = ""

#Create an initial default config file, if it doesn't exist yet
def CheckAndCreateConfig():    
    if not os.path.exists(os.path.expanduser('~/MexTKGUI_config.ini')):
        config.read(os.path.expanduser('~/MexTKGUI_config.ini'))
        config.set('main', 'ftCScriptPath', 'test')
        config.set('main', 'ftFunctionChkBox', 'false')
        config.set('main', 'ftFunctionPath', '')
        config.set('main', 'ftDATChkBox', 'true')
        config.set('main', 'ftDATPath', '')
        config.set('main', 'itCScriptPath', '')
        config.set('main', 'itID', '0')
        config.set('main', 'itFunctionChkBox', 'false')
        config.set('main', 'itFunctionPath', '')
        config.set('main', 'itDATChkBox', 'true')
        config.set('main', 'itDATPath', '')
        config.set('main', 'grCScriptPath', '')
        config.set('main', 'grFunctionChkBox', 'false')
        config.set('main', 'grFunctionPath', '')
        config.set('main', 'grDATChkBox', 'true')
        config.set('main', 'grDATPath', '')
        config.set('dolphin', 'DolphinPath', '')
        config.set('dolphin', 'FSdolPath', '')
        config.set('dolphin', 'Log', 'false')
        config.set('dolphin', 'Debug', 'false')
        config.set('dolphin', 'AutoExit', 'false')
        config.set('mextk', 'MexTKPath', '')
        with open(os.path.expanduser('~/MexTKGUI_config.ini'), 'w') as f:
            config.write(f) 

#Write a Config file in the specified path
def WriteConfigFile(configpath):
        config.read(configpath)
        config.set('main', 'ftCScriptPath', ui.txedit_CodePath_Fighter.text())
        config.set('main', 'ftFunctionChkBox', str(ui.chkbox_FunctionEnable_Fighter.isChecked()))
        config.set('main', 'ftFunctionPath', ui.txedit_FunctionPath_Fighter.text())
        config.set('main', 'ftDATChkBox', str(ui.chkbox_DatEnable_Fighter.isChecked()))
        config.set('main', 'ftDATPath', ui.txedit_DatPath_Fighter.text())
        config.set('main', 'itCScriptPath', ui.txedit_CodePath_Item.text())
        config.set('main', 'itID', str(ui.spinBox_ItemID.value()))
        config.set('main', 'itFunctionChkBox', str(ui.chkbox_FunctionEnable_Item.isChecked()))
        config.set('main', 'itFunctionPath', ui.txedit_FunctionPath_Item.text())
        config.set('main', 'itDATChkBox', str(ui.chkbox_DatEnable_Item.isChecked()))
        config.set('main', 'itDATPath', ui.txedit_DatPath_Item.text())
        config.set('main', 'grCScriptPath',  ui.txedit_CodePath_Stage.text())
        config.set('main', 'grFunctionChkBox', str(ui.chkbox_FunctionEnable_Stage.isChecked()))
        config.set('main', 'grFunctionPath', ui.txedit_FunctionPath_Stage.text())
        config.set('main', 'grDATChkBox', str(ui.chkbox_DatEnable_Stage.isChecked()))
        config.set('main', 'grDATPath', ui.txedit_DatPath_Stage.text())
        config.set('dolphin', 'DolphinPath', dolphinui.txedit_DolphinPath.text())
        config.set('dolphin', 'FSdolPath', dolphinui.txedit_FSdolPath.text())
        config.set('dolphin', 'Log', str(dolphinui.chkbox_Logger.isChecked()))
        config.set('dolphin', 'Debug', str(dolphinui.chkbox_Debugger.isChecked()))
        config.set('dolphin', 'AutoExit', str(dolphinui.chkbox_AutoExit.isChecked()))
        config.set('mextk', 'MexTKPath', mextkui.txedit_MexTKPath.text())
        with open(configpath, 'w') as f:
            config.write(f) 

#Parse a path for any spaces, if spaces exist, makes the path be enclosed in quotes
def ParsePathForSpacesAddQuotes(CodePath):
    if ' ' in  CodePath:
        CodePath = '"'+CodePath+'"'
    return CodePath

#Parse multiple paths for any spaces, if spaces exist, makes each path be enclosed in quotes
def ParsePathsForSpacesAddQuotes(CodePaths):
    for i in range(len(CodePaths)):
        if ' ' in  CodePaths[i]:
            CodePaths[i] = '"'+CodePaths[i]+'"'
    return CodePaths



if __name__ == "__main__":
    #Initialise UI
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    DolphinDialog = QtWidgets.QDialog()
    MexTKDialog = QtWidgets.QDialog()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    dolphinui = dolphin_config.Ui_Dialog()
    dolphinui.setupUi(DolphinDialog)
    mextkui = mextk_config.Ui_Dialog()
    mextkui.setupUi(MexTKDialog)
    #Initialise cofig files
    config = ConfigParser()
    config.add_section('main')
    config.add_section('dolphin')
    config.add_section('mextk')
    CheckAndCreateConfig()
    ConfigVars = configvars()
    
    #Functions

    #Fill all forms with the specified variables
    def FillForms(ConfigVars):
        ui.txedit_CodePath_Fighter.setText(ConfigVars.ftCScriptPath)
        ui.chkbox_FunctionEnable_Fighter.setChecked(ConfigVars.ftFunctionChkBox)
        ui.txedit_FunctionPath_Fighter.setText(ConfigVars.ftFunctionPath)
        ui.chkbox_DatEnable_Fighter.setChecked(ConfigVars.ftDATChkBox)
        ui.txedit_DatPath_Fighter.setText(ConfigVars.ftDATPath)
        ui.txedit_CodePath_Item.setText(ConfigVars.itCScriptPath)
        ui.chkbox_FunctionEnable_Item.setChecked(ConfigVars.itFunctionChkBox)
        ui.txedit_FunctionPath_Item.setText(ConfigVars.itFunctionPath)
        ui.spinBox_ItemID.setValue(ConfigVars.itID)
        ui.chkbox_DatEnable_Item.setChecked(ConfigVars.itDATChkBox)
        ui.txedit_DatPath_Item.setText(ConfigVars.itDATPath)
        ui.txedit_CodePath_Stage.setText(ConfigVars.grCScriptPath)
        ui.chkbox_FunctionEnable_Stage.setChecked(ConfigVars.grFunctionChkBox)
        ui.txedit_FunctionPath_Stage.setText(ConfigVars.grFunctionPath)
        ui.chkbox_DatEnable_Stage.setChecked(ConfigVars.grDATChkBox)
        ui.txedit_DatPath_Stage.setText(ConfigVars.grDATPath)
        dolphinui.txedit_DolphinPath.setText(ConfigVars.DolphinPath)
        dolphinui.txedit_FSdolPath.setText(ConfigVars.FSdolPath)
        dolphinui.chkbox_Logger.setChecked(ConfigVars.Log)
        dolphinui.chkbox_Debugger.setChecked(ConfigVars.Debug)
        dolphinui.chkbox_AutoExit.setChecked(ConfigVars.AutoExit)
        mextkui.txedit_MexTKPath.setText(ConfigVars.MexTKPath)
    
    #Load a config file specified in parameters
    def LoadConfigFile(configpath, ConfigVars):
        config.read(configpath)
        ConfigVars.ftCScriptPath = config.get('main', 'ftCScriptPath')
        ConfigVars.ftFunctionChkBox = config.getboolean('main', 'ftFunctionChkBox')
        ConfigVars.ftFunctionPath = config.get('main', 'ftFunctionPath')
        ConfigVars.ftDATChkBox = config.getboolean('main', 'ftDATChkBox')
        ConfigVars.ftDATPath = config.get('main', 'ftDATPath')
        ConfigVars.itCScriptPath = config.get('main', 'itCScriptPath')
        ConfigVars.itID = config.getint('main', 'itID')
        ConfigVars.itFunctionChkBox = config.getboolean('main', 'itFunctionChkBox')
        ConfigVars.itFunctionPath = config.get('main', 'itFunctionPath')
        ConfigVars.itDATChkBox = config.getboolean('main', 'itDATChkBox')
        ConfigVars.itDATPath = config.get('main', 'itDATPath')
        ConfigVars.grCScriptPath = config.get('main', 'grCScriptPath')
        ConfigVars.grFunctionChkBox = config.getboolean('main', 'grFunctionChkBox')
        ConfigVars.grFunctionPath = config.get('main', 'grFunctionPath')
        ConfigVars.grDATChkBox = config.getboolean('main', 'grDATChkBox')
        ConfigVars.grDATPath = config.get('main', 'grDATPath')
        ConfigVars.DolphinPath = config.get('dolphin', 'DolphinPath')
        ConfigVars.FSdolPath = config.get('dolphin', 'FSdolPath')
        ConfigVars.Log = config.getboolean('dolphin', 'Log')
        ConfigVars.Debug = config.getboolean('dolphin', 'Debug')
        ConfigVars.AutoExit = config.getboolean('dolphin', 'AutoExit')
        ConfigVars.MexTKPath = config.get('mextk', 'MexTKPath')
        FillForms(ConfigVars)

    #File select for the Fighter code File
    def selectCodeFileFighter():
        CodePaths = QFileDialog.getOpenFileNames(ui.txedit_CodePath_Fighter,'Open Item C scripts','','C scripts (*.c)')[0]
        CodePaths = ParsePathsForSpacesAddQuotes(CodePaths)
        ui.txedit_CodePath_Fighter.setText(' '.join(CodePaths))
    
    #File select for the Fighter ftFunction DAT
    def selectFunctionFileFighter():
        CodePath = QFileDialog.getSaveFileName(ui.txedit_CodePath_Fighter,'Save ftFunction.dat','','ftFunction.dat (*.dat)')[0]
        CodePath = ParsePathForSpacesAddQuotes(CodePath)
        ui.txedit_FunctionPath_Fighter.setText(CodePath)

    #File select for the Fighter DAT File
    def selectDatFileFighter():
        CodePath = QFileDialog.getOpenFileName(ui.txedit_CodePath_Fighter,'Open Fighter DAT','','Fighter DAT (*.dat)')[0]
        CodePath = ParsePathForSpacesAddQuotes(CodePath)
        ui.txedit_DatPath_Fighter.setText(CodePath)

    #Compile Fighter function
    def RunMexTKFighter():
        if "MexTK.exe" not in mextkui.txedit_MexTKPath.text():
                msg = QMessageBox()
                msg.setIcon(QMessageBox.Warning)
                msg.setText("MexTK not configured!")
                msg.setInformativeText('You should fill in the MexTK path in MexTK settings window, lemme open that for you')
                msg.setWindowTitle("Warning")
                msg.exec_()
                OpenMexTKSettings()
        else:
            CommandString = mextkui.txedit_MexTKPath.text() +' -ff -i '+ ui.txedit_CodePath_Fighter.text() +' -s ftFunction'
            if ui.chkbox_DatEnable_Fighter.isChecked():
                CommandString += ' -dat ' + ui.txedit_DatPath_Fighter.text()
            if ui.chkbox_FunctionEnable_Fighter.isChecked():
                CommandString += ' -o '+ ui.txedit_FunctionPath_Fighter.text() +' -ow'
            if (not ui.chkbox_DatEnable_Fighter.isChecked()) and  (not ui.chkbox_FunctionEnable_Fighter.isChecked()):
                msg = QMessageBox()
                msg.setIcon(QMessageBox.Warning)
                msg.setText("No Output!")
                msg.setInformativeText('You should enable at least one output file')
                msg.setWindowTitle("Warning")
                msg.exec_()
                
            return_value =subprocess.getoutput(CommandString + ' -ow')
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)
            msg.setText("MexTK Result:")
            msg.setInformativeText(return_value)
            msg.setWindowTitle("MexTK Result")
            msg.exec_()

    #File select for the Item code File
    def selectCodeFileItem():
        CodePaths = QFileDialog.getOpenFileNames(ui.txedit_CodePath_Item,'Open Item C scripts','','C scripts (*.c)')[0]
        CodePaths = ParsePathsForSpacesAddQuotes(CodePaths)
        ui.txedit_CodePath_Item.setText(' '.join(CodePaths))
    
    #File select for the Item itFunction DAT
    def selectFunctionFileItem():
        CodePath = QFileDialog.getSaveFileName(ui.txedit_CodePath_Item,'Save itFunction.dat','','itFunction.dat (*.dat)')[0]
        CodePath = ParsePathForSpacesAddQuotes(CodePath)
        ui.txedit_FunctionPath_Item.setText(CodePath)
        
    #File select for the Item DAT File
    def selectDatFileItem():
        CodePath = QFileDialog.getOpenFileName(ui.txedit_CodePath_Item,'Open Fighter DAT','','Fighter DAT (*.dat)')[0]
        CodePath = ParsePathForSpacesAddQuotes(CodePath)
        ui.txedit_DatPath_Item.setText(CodePath)
    #Compile Item function
    def RunMexTKItem():
        if "MexTK.exe" not in mextkui.txedit_MexTKPath.text():
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Warning)
            msg.setText("MexTK not configured!")
            msg.setInformativeText('You should fill in the MexTK path in MexTK settings window, lemme open that for you')
            msg.setWindowTitle("Warning")
            msg.exec_()
            OpenMexTKSettings()
        else:
            CommandString = mextkui.txedit_MexTKPath.text() +' -ff -item '+ ui.spinBox_ItemID.text() + " "+ ui.txedit_CodePath_Item.text() +' -s itFunction'
            if ui.chkbox_DatEnable_Item.isChecked():
                CommandString += ' -dat ' + ui.txedit_DatPath_Item.text()
            if ui.chkbox_FunctionEnable_Item.isChecked():
                CommandString += ' -o '+ ui.txedit_FunctionPath_Item.text() +' -ow'
            if (not ui.chkbox_DatEnable_Item.isChecked()) and  (not ui.chkbox_FunctionEnable_Item.isChecked()):
                msg = QMessageBox()
                msg.setIcon(QMessageBox.Warning)
                msg.setText("No Output!")
                msg.setInformativeText('You should enable at least one output file')
                msg.setWindowTitle("Warning")
                msg.exec_()
            
            return_value =subprocess.getoutput(CommandString + ' -ow')
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)
            msg.setText("MexTK Result:")
            msg.setInformativeText(return_value)
            msg.setWindowTitle("MexTK Result")
            msg.exec_()


    #File select for the Stage code File
    def selectCodeFileStage():
        CodePaths = QFileDialog.getOpenFileNames(ui.txedit_CodePath_Fighter,'Open Stage C scripts','','C scripts (*.c)')[0]
        CodePaths = ParsePathsForSpacesAddQuotes(CodePaths)
        ui.txedit_CodePath_Stage.setText(' '.join(CodePaths))
   
    #File select for the Stage grFunction DAT
    def selectFunctionFileStage():
        CodePath = QFileDialog.getSaveFileName(ui.txedit_CodePath_Stage,'Save grFunction.dat','','grFunction.dat (*.dat)')[0]
        CodePath = ParsePathForSpacesAddQuotes(CodePath)
        ui.txedit_FunctionPath_Stage.setText(CodePath)

    #File select for the Stage DAT File
    def selectDatFileStage():
        CodePath = QFileDialog.getOpenFileName(ui.txedit_CodePath_Stage,'Open Fighter DAT','','Fighter DAT (*.dat)')[0]
        CodePath = ParsePathForSpacesAddQuotes(CodePath)
        ui.txedit_DatPath_Stage.setText(CodePath)
    
    #Compile Stage Function
    def RunMexTKStage():
        if "MexTK.exe" not in mextkui.txedit_MexTKPath.text():
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Warning)
            msg.setText("MexTK not configured!")
            msg.setInformativeText('You should fill in the MexTK path in MexTK settings window, lemme open that for you')
            msg.setWindowTitle("Warning")
            msg.exec_()
            OpenMexTKSettings()
        else:
            CommandString = mextkui.txedit_MexTKPath.text() + ' -ff -i '+ ui.txedit_CodePath_Stage.text() +' -s grFunction' 
            if ui.chkbox_DatEnable_Stage.isChecked():
                CommandString += ' -dat ' + ui.txedit_DatPath_Stage.text()
            if ui.chkbox_FunctionEnable_Stage.isChecked():
                CommandString += ' -o '+ ui.txedit_FunctionPath_Stage.text() +' -ow'
            if (not ui.chkbox_DatEnable_Stage.isChecked()) and  (not ui.chkbox_FunctionEnable_Stage.isChecked()):
                msg = QMessageBox()
                msg.setIcon(QMessageBox.Warning)
                msg.setText("No Output!")
                msg.setInformativeText('You should enable at least one output file')
                msg.setWindowTitle("Warning")
                msg.exec_()

            return_value =subprocess.getoutput(CommandString + ' -ow')
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)
            msg.setText("MexTK Result:")
            msg.setInformativeText(return_value)
            msg.setWindowTitle("MexTK Result")
            msg.exec_()

    
    #Function to open dolphin settings window
    def OpenDolphinSettings():
        DolphinDialog.show()
    
    #File select for dolphin executable
    def SelectDolphinFile():
        DolphinPath = QFileDialog.getOpenFileName(dolphinui.txedit_DolphinPath,'Open Dolphin EXE','','Dolphin exe (*.exe)')[0]
        if ' ' in DolphinPath:
            dolphinui.txedit_DolphinPath.setText('"'+DolphinPath+'"')
        else:
            dolphinui.txedit_DolphinPath.setText(DolphinPath)
    
    #File select for melee's filesystem main.dol file
    def SelectFSdolFile():
        FSdolPath = QFileDialog.getOpenFileName(dolphinui.txedit_DolphinPath,'Open Filesystem main.dol','','main.dol (main.dol)')[0]
        if ' ' in FSdolPath:
            dolphinui.txedit_FSdolPath.setText('"'+FSdolPath+'"')
        else:
            dolphinui.txedit_FSdolPath.setText(FSdolPath)

    def OpenMexTKSettings():
        MexTKDialog.show()  

    #File select for dolphin executable
    def SelectMexTKFile():
        MexTKPath = QFileDialog.getOpenFileName(mextkui.txedit_MexTKPath,'Open MexTK EXE','','MexTK exe (MexTK.exe)')[0]
        if ' ' in MexTKPath:
            mextkui.txedit_MexTKPath.setText('"'+MexTKPath+'"')
        else:
            mextkui.txedit_MexTKPath.setText(MexTKPath)
    #Run dolphin and play the main.dol
    def RunDolphin():
        parameters = ""
        if dolphinui.chkbox_Logger.isChecked():
            parameters += " -l"
        if dolphinui.chkbox_Debugger.isChecked():
            parameters += " -d"
        if dolphinui.chkbox_AutoExit.isChecked():
            parameters += " -b"
        #Dolphin = QProcess()
        # Dolphin.startDetached(dolphinui.txedit_DolphinPath.text()+ parameters + ' -e '+dolphinui.txedit_FSdolPath.text())
        #Dolphin.start()
        subprocess.Popen(dolphinui.txedit_DolphinPath.text()+ parameters + ' -e '+dolphinui.txedit_FSdolPath.text(), creationflags = subprocess.CREATE_NEW_PROCESS_GROUP, close_fds = True)
        #os.popen(dolphinui.txedit_DolphinPath.text()+ parameters + " -e "+dolphinui.txedit_FSdolPath.text())
        #os.system('cmd /c '+dolphinui.txedit_DolphinPath.text()+ parameters + " -e "+dolphinui.txedit_FSdolPath.text())
    
    #Save a model for later use
    def SaveModel():
        filepath = QFileDialog.getSaveFileName(ui.txedit_DatPath_Stage,'Save model INI','','Model INI (*.ini)')[0]
        WriteConfigFile(filepath)
    
    #load a previously saved model
    def OpenModel():
        filepath = QFileDialog.getOpenFileName(ui.txedit_DatPath_Stage,'Open model INI','','Model INI (*.ini)')[0]
        LoadConfigFile(filepath,ConfigVars)
    
    #Read default Config
    LoadConfigFile(os.path.expanduser('~/MexTKGUI_config.ini'),ConfigVars)
    
    #Initialise Fighter UI functions
    ui.btn_SelectCodeFile_Fighter.clicked.connect(selectCodeFileFighter)
    ui.btn_SelectFunctionFile_Fighter.clicked.connect(selectFunctionFileFighter)
    ui.btn_SelectDatFile_Fighter.clicked.connect(selectDatFileFighter)
    ui.btn_Compile_Fighter.clicked.connect(RunMexTKFighter)
    ui.chkbox_DatEnable_Fighter.toggled.connect(ui.txedit_DatPath_Fighter.setEnabled)
    ui.chkbox_DatEnable_Fighter.toggled.connect(ui.btn_SelectDatFile_Fighter.setEnabled)
    ui.chkbox_FunctionEnable_Fighter.toggled.connect(ui.txedit_FunctionPath_Fighter.setEnabled)
    ui.chkbox_FunctionEnable_Fighter.toggled.connect(ui.btn_SelectFunctionFile_Fighter.setEnabled)
    
    #Initialise Item UI functions
    ui.btn_SelectCodeFile_Item.clicked.connect(selectCodeFileItem)
    ui.btn_SelectFunctionFile_Item.clicked.connect(selectFunctionFileItem)
    ui.btn_SelectDatFile_Item.clicked.connect(selectDatFileItem)
    ui.btn_Compile_Item.clicked.connect(RunMexTKItem)
    ui.chkbox_DatEnable_Item.toggled.connect(ui.txedit_DatPath_Item.setEnabled)
    ui.chkbox_DatEnable_Item.toggled.connect(ui.btn_SelectDatFile_Item.setEnabled)
    ui.chkbox_FunctionEnable_Item.toggled.connect(ui.txedit_FunctionPath_Item.setEnabled)
    ui.chkbox_FunctionEnable_Item.toggled.connect(ui.btn_SelectFunctionFile_Item.setEnabled)
    
    #Initialise Stage UI functions
    ui.btn_SelectCodeFile_Stage.clicked.connect(selectCodeFileStage)
    ui.btn_SelectFunctionFile_Stage.clicked.connect(selectFunctionFileStage)
    ui.btn_SelectDatFile_Stage.clicked.connect(selectDatFileStage)
    ui.btn_Compile_Stage.clicked.connect(RunMexTKStage)
    ui.chkbox_DatEnable_Stage.toggled.connect(ui.txedit_DatPath_Stage.setEnabled)
    ui.chkbox_DatEnable_Stage.toggled.connect(ui.btn_SelectDatFile_Stage.setEnabled)
    ui.chkbox_FunctionEnable_Stage.toggled.connect(ui.txedit_FunctionPath_Stage.setEnabled)
    ui.chkbox_FunctionEnable_Stage.toggled.connect(ui.btn_SelectFunctionFile_Stage.setEnabled)
    
    #Initialise File menu
    ui.actionDolphin_settings.triggered.connect(OpenDolphinSettings)
    ui.actionMexTK_settings.triggered.connect(OpenMexTKSettings)
    ui.actionSave_filled_content_to_model.triggered.connect(SaveModel)
    ui.actionOpen_model.triggered.connect(OpenModel)
   
    #Initialise Run Dolphin button
    ui.btn_DolphinTest.clicked.connect(RunDolphin)
    
    #Dolphin UI functions
    dolphinui.btn_SelectDolphinFile.clicked.connect(SelectDolphinFile)
    dolphinui.btn_SelectFSdolFile.clicked.connect(SelectFSdolFile)
   
    #MexTK UI functions
    mextkui.btn_SelectMexTKFile.clicked.connect(SelectMexTKFile)
    #Fill forms with .ini values
    FillForms(ConfigVars)
    
    #Show the Main UI window
    MainWindow.show()
    
    #Define what the app does on exit
    ret = app.exec_()
    
    #Writes every current variables to the config file
    WriteConfigFile(os.path.expanduser('~/MexTKGUI_config.ini'))
    
    sys.exit(ret)
