function write_norm(filename, norm)
    fileID = fopen(filename, 'w');
    fprintf(fileID,'%.15f\n',norm);
    fclose(fileID);
end