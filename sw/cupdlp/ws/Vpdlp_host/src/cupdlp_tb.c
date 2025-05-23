// #include "mps_lp.h"
// #include "wrapper_highs.h"

// cupdlp_retcode main(int argc, char **argv) {
//     cupdlp_retcode retcode = RETCODE_OK;

//     char *fname = "./example/afiro.mps";
//     char *fout = "./solution-sum.json";
//     char *fout_sol = "./solution.json";

//     cupdlp_bool ifSaveSol = false;
//     cupdlp_bool ifPresolve = false;

//     int nCols_pdlp = 0;
//     int nRows_pdlp = 0;
//     int nEqs_pdlp = 0;
//     int nnz_pdlp = 0;
//     int status_pdlp = -1;

//     cupdlp_float *rhs = NULL;
//     cupdlp_float *cost = NULL;
//     cupdlp_float *lower = NULL;
//     cupdlp_float *upper = NULL;

//     int *csc_beg = NULL, *csc_idx = NULL;
//     double *csc_val = NULL;

//     double offset = 0.0;
//     double sense = 1;
//     int *constraint_new_idx = NULL;
//     int *constraint_type = NULL;

//     int nCols = 0;
//     cupdlp_float *col_value = cupdlp_NULL;
//     cupdlp_float *col_dual = cupdlp_NULL;
//     cupdlp_float *row_value = cupdlp_NULL;
//     cupdlp_float *row_dual = cupdlp_NULL;

//     int nCols_org = 0;
//     int nRows_org = 0;
//     cupdlp_float *col_value_org = cupdlp_NULL;
//     cupdlp_float *col_dual_org = cupdlp_NULL;
//     cupdlp_float *row_value_org = cupdlp_NULL;
//     cupdlp_float *row_dual_org = cupdlp_NULL;

//     int nCols_pre = 0;
//     int nRows_pre = 0;
//     cupdlp_float *col_value_pre = cupdlp_NULL;
//     cupdlp_float *col_dual_pre = cupdlp_NULL;
//     cupdlp_float *row_value_pre = cupdlp_NULL;
//     cupdlp_float *row_dual_pre = cupdlp_NULL;
//     cupdlp_int value_valid = 0;
//     cupdlp_int dual_valid = 0;

//     void *model = NULL;
//     void *presolvedmodel = NULL;
//     int presolve_status = -1;
//     void *model2solve = NULL;

//     CUPDLPscaling *scaling =
//         (CUPDLPscaling *)cupdlp_malloc(sizeof(CUPDLPscaling));

//     CUPDLP_MATRIX_FORMAT src_matrix_format = CSC;
//     CUPDLP_MATRIX_FORMAT dst_matrix_format = CSR_CSC;
//     CUPDLPcsc *csc_cpu = cupdlp_NULL;
//     CUPDLPproblem *prob = cupdlp_NULL;

//     for (cupdlp_int i = 0; i < argc - 1; i++) {
//         if (strcmp(argv[i], "-fname") == 0) {
//             fname = argv[i + 1];
//         } else if (strcmp(argv[i], "-out") == 0) {
//             fout = argv[i + 1];
//         } else if (strcmp(argv[i], "-h") == 0) {
//             print_script_usage();
//             break;
//         } else if (strcmp(argv[i], "-savesol") == 0) {
//             ifSaveSol = atoi(argv[i + 1]);
//         } else if (strcmp(argv[i], "-ifPre") == 0) {
//             ifPresolve = atoi(argv[i + 1]);
//         } else if (strcmp(argv[i], "-outSol") == 0) {
//             fout_sol = argv[i + 1];
//         }
//     }
//     if (strcmp(argv[argc - 1], "-h") == 0) {
//         print_script_usage();
//     }

//     cupdlp_bool ifChangeIntParam[N_INT_USER_PARAM] = {false};
//     cupdlp_int intParam[N_INT_USER_PARAM] = {0};
//     cupdlp_bool ifChangeFloatParam[N_FLOAT_USER_PARAM] = {false};
//     cupdlp_float floatParam[N_FLOAT_USER_PARAM] = {0.0};
//     CUPDLP_CALL(getUserParam(argc, argv, ifChangeIntParam, intParam,
//                              ifChangeFloatParam, floatParam));

//     model = createModel_highs();
//     CUPDLP_CALL(loadMps_highs(model, fname));
//     getModelSize_highs(model, &nCols_org, &nRows_org, NULL);
//     nCols = nCols_org;

//     model2solve = model;

//     if (ifChangeIntParam[IF_PRESOLVE]) {
//         ifPresolve = intParam[IF_PRESOLVE];
//     }

//     cupdlp_float presolve_time = getTimeStamp();
//     if (ifPresolve) {
//         presolvedmodel = createModel_highs();
//         presolve_status = presolvedModel_highs(presolvedmodel, model);
//         getModelSize_highs(presolvedmodel, &nCols_pre, &nRows_pre, NULL);
//         if (presolve_status == 2) {
//             cupdlp_printf(
//                 "Infeasible or Unbounded LP detected by HiGHS presolver.\n");
//             writeJsonFromHiGHS_highs(fout, model);
//             if (ifSaveSol) {
//                 printf("--- no sol file saved.\n");
//             }
//             goto exit_cleanup;
//         } else if (presolve_status == 3) {
//             cupdlp_printf("Solved by HiGHS presolver.\n");
//             postsolveModelFromEmpty_highs(model);
//             writeJsonFromHiGHS_highs(fout, model);
//             if (ifSaveSol) {
//                 writeSolFromHiGHS_highs(fout_sol, model);
//             }
//             goto exit_cleanup;
//         }
//         model2solve = presolvedmodel;
//         nCols = nCols_pre;
//     }
//     presolve_time = getTimeStamp() - presolve_time;

//     CUPDLP_CALL(formulateLP_highs(model2solve, &cost, &nCols_pdlp, &nRows_pdlp,
//                                   &nnz_pdlp, &nEqs_pdlp, &csc_beg, &csc_idx,
//                                   &csc_val, &rhs, &lower, &upper, &offset, &sense,
//                                   &nCols, &constraint_new_idx, &constraint_type));

//     CUPDLP_CALL(Init_Scaling(scaling, nCols_pdlp, nRows_pdlp, cost, rhs));
//     cupdlp_int ifScaling = 1;

//     if (ifChangeIntParam[IF_SCALING]) {
//         ifScaling = intParam[IF_SCALING];
//     }

//     if (ifChangeIntParam[IF_RUIZ_SCALING]) {
//         scaling->ifRuizScaling = intParam[IF_RUIZ_SCALING];
//     }

//     if (ifChangeIntParam[IF_L2_SCALING]) {
//         scaling->ifL2Scaling = intParam[IF_L2_SCALING];
//     }

//     if (ifChangeIntParam[IF_PC_SCALING]) {
//         scaling->ifPcScaling = intParam[IF_PC_SCALING];
//     }

//     // the work object needs to be established first
//     // free inside cuPDLP
//     CUPDLPwork *w = cupdlp_NULL;
//     CUPDLP_INIT_ZERO(w, 1);
// #if !(CUPDLP_CPU)
//     // cupdlp_float cuda_prepare_time = getTimeStamp();
//     // CHECK_CUSPARSE(cusparseCreate(&w->cusparsehandle));
//     // CHECK_CUBLAS(cublasCreate(&w->cublashandle));
//     // cuda_prepare_time = getTimeStamp() - cuda_prepare_time;
// #endif

//     CUPDLP_CALL(problem_create(&prob));

//     // currently, only supprot that input matrix is CSC, and store both CSC and
//     // CSR
//     CUPDLP_CALL(csc_create(&csc_cpu));
//     csc_cpu->nRows = nRows_pdlp;
//     csc_cpu->nCols = nCols_pdlp;
//     csc_cpu->nMatElem = nnz_pdlp;
//     CUPDLP_INIT(csc_cpu->colMatBeg, 1 + nCols_pdlp)
//     CUPDLP_INIT(csc_cpu->colMatIdx, nnz_pdlp)
//     CUPDLP_INIT(csc_cpu->colMatElem, nnz_pdlp)
//     memcpy(csc_cpu->colMatBeg, csc_beg, ((size_t)nCols_pdlp + 1) * sizeof(int));
//     memcpy(csc_cpu->colMatIdx, csc_idx, nnz_pdlp * sizeof(int));
//     memcpy(csc_cpu->colMatElem, csc_val, nnz_pdlp * sizeof(double));
// #if !(CUPDLP_CPU)
//     // csc_cpu->cuda_csc = NULL;
// #endif

//     cupdlp_float scaling_time = getTimeStamp();
//     CUPDLP_CALL(PDHG_Scale_Data(csc_cpu, ifScaling, scaling, cost, lower, upper, rhs));
//     scaling_time = getTimeStamp() - scaling_time;

//     cupdlp_float alloc_matrix_time = 0.0;
//     cupdlp_float copy_vec_time = 0.0;

// exit_cleanup:
//     deleteModel_highs(model);
//     if (ifPresolve) {
//         deleteModel_highs(presolvedmodel);
//         if (col_value_pre != NULL) cupdlp_free(col_value_pre);
//         if (col_dual_pre != NULL) cupdlp_free(col_dual_pre);
//         if (row_value_pre != NULL) cupdlp_free(row_value_pre);
//         if (row_dual_pre != NULL) cupdlp_free(row_dual_pre);
//     }
//     if (col_value_org != NULL) cupdlp_free(col_value_org);
//     if (col_dual_org != NULL) cupdlp_free(col_dual_org);
//     if (row_value_org != NULL) cupdlp_free(row_value_org);
//     if (row_dual_org != NULL) cupdlp_free(row_dual_org);
//     col_value = NULL;
//     col_dual = NULL;
//     row_value = NULL;
//     row_dual = NULL;

//     if (scaling) {
//         scaling_clear(scaling);
//     }

//     if (cost != NULL) cupdlp_free(cost);
//     if (csc_beg != NULL) cupdlp_free(csc_beg);
//     if (csc_idx != NULL) cupdlp_free(csc_idx);
//     if (csc_val != NULL) cupdlp_free(csc_val);
//     if (rhs != NULL) cupdlp_free(rhs);
//     if (lower != NULL) cupdlp_free(lower);
//     if (upper != NULL) cupdlp_free(upper);
//     if (constraint_new_idx != NULL) cupdlp_free(constraint_new_idx);
//     if (constraint_type != NULL) cupdlp_free(constraint_type);


//     return retcode;
// }
