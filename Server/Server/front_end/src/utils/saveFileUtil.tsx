import { saveAs } from 'file-saver';

export const saveTextFile = (fileName:string, fileContent:string) =>{
    const blob = new Blob([fileContent], {type: "text/plain;charset=utf-8"});
    saveAs(blob, fileName);
}