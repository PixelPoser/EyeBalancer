document.addEventListener('DOMContentLoaded', () => {
    const selectFolderBtn = document.getElementById('select-folder');
    const fullCropBtn = document.getElementById('full-crop');
    const squareCropBtn = document.getElementById('square-crop');
    const balanceEyesBtn = document.getElementById('balance-eyes');
    const dropZone = document.getElementById('drop-zone');
    const progressContainer = document.getElementById('progress-container');
    const progressBar = document.getElementById('progress-bar');
    const progressText = document.getElementById('progress-text');
    let selectedFolderPath = '';
  
    selectFolderBtn.addEventListener('click', () => {
      window.api.send('open-dialog');
    });
  
    fullCropBtn.addEventListener('click', () => {
      if (selectedFolderPath) {
        window.api.send('process-folder', { folderPath: selectedFolderPath, cropOption: 'full-crop' });
        disableCropButtons();
        showProgressContainer();
      }
    });
  
    squareCropBtn.addEventListener('click', () => {
      if (selectedFolderPath) {
        window.api.send('process-folder', { folderPath: selectedFolderPath, cropOption: 'square-crop' });
        disableCropButtons();
        showProgressContainer();
      }
    });
  
    balanceEyesBtn.addEventListener('click', () => {
      if (selectedFolderPath) {
        window.api.send('process-folder', { folderPath: selectedFolderPath, cropOption: 'balance-eyes' });
        disableCropButtons();
        showProgressContainer();
      }
    });
  
    dropZone.addEventListener('dragover', (event) => {
      event.preventDefault();
      event.stopPropagation();
      event.dataTransfer.dropEffect = 'copy';
    });
  
    dropZone.addEventListener('drop', (event) => {
      event.preventDefault();
      event.stopPropagation();
      const folderPath = event.dataTransfer.files[0].path;
      if (folderPath) {
        selectedFolderPath = folderPath;
        enableCropButtons();
        console.log(`Dropped folder: ${folderPath}`);
      }
    });
  
    window.api.receive('selected-directory', (path) => {
      if (path) {
        selectedFolderPath = path;
        enableCropButtons();
        console.log(`Selected folder: ${path}`);
      }
    });
  
    window.api.receive('process-progress', (progress) => {
      progressBar.value = progress;
      progressText.textContent = `Processing... ${progress}%`;
    });
  
    window.api.receive('process-complete', () => {
      hideProgressContainer();
      enableCropButtons();
      alert('Processing complete!');
    });
  
    window.api.receive('process-error', (error) => {
      hideProgressContainer();
      enableCropButtons();
      alert(`Error during processing: ${error}`);
    });
  
    window.api.receive('show-xmp-prompt', () => {
      const outputXMPForJPEGs = window.confirm('Do you want to output XMP files for JPEGs?');
      window.api.send('xmp-prompt-response', outputXMPForJPEGs);
    });
  
    function enableCropButtons() {
      fullCropBtn.disabled = false;
      squareCropBtn.disabled = false;
      balanceEyesBtn.disabled = false;
    }
  
    function disableCropButtons() {
      fullCropBtn.disabled = true;
      squareCropBtn.disabled = true;
      balanceEyesBtn.disabled = true;
    }
  
    function showProgressContainer() {
      progressContainer.style.display = 'block';
    }
  
    function hideProgressContainer() {
      progressContainer.style.display = 'none';
    }
  });