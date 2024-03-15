const { app, BrowserWindow, dialog, ipcMain } = require('electron');
const path = require('path');
const { execFile } = require('child_process');
const fs = require('fs');

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: false,
      contextIsolation: true,
    },
  });

  mainWindow.loadFile('index.html');

  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

app.whenReady().then(createWindow);

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

ipcMain.on('open-dialog', async (event) => {
  const { canceled, filePaths } = await dialog.showOpenDialog(mainWindow, {
    properties: ['openDirectory']
  });
  if (!canceled && filePaths.length > 0) {
    event.reply('selected-directory', filePaths[0]);
  }
});

ipcMain.on('process-folder', (event, { folderPath, cropOption }) => {
  const hasNonRawFiles = checkForNonRawFiles(folderPath);

  if (hasNonRawFiles) {
    mainWindow.webContents.send('show-xmp-prompt');

    ipcMain.once('xmp-prompt-response', (event, outputXMPForJPEGs) => {
      processFolderWithXMPOption(folderPath, outputXMPForJPEGs, cropOption);
    });
  } else {
    processFolderWithXMPOption(folderPath, false, cropOption);
  }
});

function checkForNonRawFiles(folderPath) {
  const files = fs.readdirSync(folderPath);
  const nonRawExtensions = ['.jpg', '.jpeg', '.png', '.heic'];

  for (const file of files) {
    const extension = path.extname(file).toLowerCase();
    if (nonRawExtensions.includes(extension)) {
      return true;
    }
  }

  return false;
}

function processFolderWithXMPOption(folderPath, outputXMPForJPEGs, cropOption) {
  const pythonExecutablePath = app.isPackaged
    ? path.join(process.resourcesPath, 'app', 'python', 'python')
    : 'python';

  let scriptName;
  switch (cropOption) {
    case 'full-crop':
      scriptName = 'eyebalancerelectron_fullcrop.py';
      break;
    case 'square-crop':
      scriptName = 'eyebalancerelectron_squarecrop.py';
      break;
    case 'balance-eyes':
      scriptName = 'eyebalancerelectron_balanceeyes.py';
      break;
    default:
      scriptName = 'eyebalancerelectron_fullcrop.py';
  }

  const scriptPath = app.isPackaged
    ? path.join(process.resourcesPath, 'app', 'scripts', scriptName)
    : path.join(__dirname, 'scripts', scriptName);

  const args = [scriptPath, folderPath, outputXMPForJPEGs.toString()];

  const childProcess = execFile(pythonExecutablePath, args);

  childProcess.stdout.on('data', (data) => {
    const progress = parseInt(data.toString().trim());
    mainWindow.webContents.send('process-progress', progress);
  });
  
  childProcess.on('close', (code) => {
    if (code === 0) {
      mainWindow.webContents.send('process-complete');
    } else {
      mainWindow.webContents.send('process-error', 'An error occurred during processing.');
    }
  });
  
}