(define (gimp-export-all output-dir stem-list)
  (while (not (null? stem-list))
         (let* ((stem (car stem-list))
                (filename-in (string-append stem ".xcf"))
                (filename-out (string-append output-dir stem ".png"))
                (image (car (gimp-file-load RUN-NONINTERACTIVE
                                           filename-in ; image-id
                                           filename-in ; drawable-id
                                           )))
                (drawable (car (gimp-image-merge-visible-layers image CLIP-TO-IMAGE))))
           (gimp-file-save RUN-NONINTERACTIVE image drawable
                           filename-out filename-out)
           (gimp-image-delete image))
         (set! stem-list (cdr stem-list)))
  (gimp-quit 0)
  )

;; Local Variables:
;;   mode:scheme
;;   coding: utf-8
;; End:
